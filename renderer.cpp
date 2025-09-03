// Author: Tate Maguire
// August 25, 2025

#include <iostream>
#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"
#include "renderer.h"

// Gets barycentric coordinates of P within the triangle defined by pts (screen coords)
// pts must have length 3
Vec3f barycentric(Vec3f* pts, Vec2i P) {
	Vec3f vx = Vec3f(pts[2].x - pts[0].x, pts[1].x - pts[0].x, pts[0].x - P.x);
	Vec3f vy = Vec3f(pts[2].y - pts[0].y, pts[1].y - pts[0].y, pts[0].y - P.y);
	Vec3f u = vx^vy; // cross product
	
	// if area is nearly zero, it's degenerate
	if (std::abs(u.z)<1) return Vec3f(-1,1,1);
	// normalize
	u = u*(1.f/u.z);
	// store results as cartesian coordinates
	Vec3f b = Vec3f(1-u.x-u.y, u.x, u.y);
	return b;
}

// triangle draw with zbuffer, model_uv, and light_level
void triangle(Vec3f screen_pos[], int* zbuffer, Vec2f vt[], TGAImage& model_uv, TGAImage& image, float light_level) {
	int w = image.get_width();
	int h = image.get_height();

	// find bounding box
	Vec2i bboxmin = Vec2i(w-1, h-1);
	Vec2i bboxmax = Vec2i(0, 0);
	for (int i=0; i<3; i++) {
		if (screen_pos[i].x < bboxmin.x) bboxmin.x = screen_pos[i].x;
		if (screen_pos[i].y < bboxmin.y) bboxmin.y = screen_pos[i].y;
		if (screen_pos[i].x > bboxmax.x) bboxmax.x = screen_pos[i].x;
		if (screen_pos[i].y > bboxmax.y) bboxmax.y = screen_pos[i].y;
	}
	if (bboxmin.x<0) bboxmin.x=0;
	if (bboxmin.y<0) bboxmin.y=0;
	if (bboxmax.x>w-1) bboxmax.x=w-1;
	if (bboxmax.y>h-1) bboxmax.y=h-1;

	// draw
	Vec2i P;
	for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
		for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
			Vec3f b = barycentric(screen_pos, P);
			const float EPS = 0;
			// if pixel is inside the triangle
			if (b.x>=-EPS && b.y>=-EPS && b.z>=-EPS) {
				int z = b * Vec3f(screen_pos[0].z, screen_pos[1].z, screen_pos[2].z);
				// if pixel is in front of the current pixel at x,y
				if (z>zbuffer[P.x+P.y*w]) {
					zbuffer[P.x+P.y*w] = z;
					float u = b * Vec3f(vt[0].u, vt[1].u, vt[2].u);
					float v = b * Vec3f(vt[0].v, vt[1].v, vt[2].v);
					TGAColor color = model_uv.get(u*model_uv.get_width(), v*model_uv.get_height());
					color = TGAColor(color.r*light_level, color.g*light_level, color.b*light_level, color.a);
					image.set(P.x, P.y, color);
				}
			}
		}
	}
}

// rasterize triangle, translate to screen coords and draw
void rasterize(Vec3f world_pos[], int* zbuffer, Vec2f vt[], TGAImage& model_uv, TGAImage& image, float light_level, float scale, Vec3f camera_pos) {
	// calculate screen positions
	for (int i=0; i<3; i++) {
		// world_pos[i].z += 1;
	}
	Vec3f screen_pos[3];
	for (int i=0; i<3; i++) {
		float coef = 1.-world_pos[i].z/(float)camera_pos.z;
		coef = 1./coef;
		screen_pos[i].x = (world_pos[i].x*coef+1)*scale;
		screen_pos[i].y = (world_pos[i].y*coef+1)*scale;
		screen_pos[i].z = (world_pos[i].z*coef+1)*scale;
	}

	triangle(screen_pos, zbuffer, vt, model_uv, image, light_level);
}

// draws the model using the light_source vector, describing light's direction as a normalized vec3f
void render(Model* model, TGAImage& model_uv, TGAImage& image, Vec3f light_source, Vec3f camera_pos) {
	int w = image.get_width();
	int h = image.get_height();
	
	// create zbuffer
	int* zbuffer = new int[w*h];
	for (int i=0; i<w; i++) {
		for (int j=0; j<h; j++) {
			zbuffer[i+j*w] = std::numeric_limits<int>::min();
		}
	}

	// calculate scale
	float scale = image.get_width()/2;

	for (int i=0; i<model->nfaces(); i++) {
        std::vector<Vec3i> f = model->face(i);
		Vec3f world_pos[3];
		Vec2f vt[3];
		for (int i=0; i<3; i++) {
			world_pos[i] = model->vert(f[i].ivert);
			vt[i] = model->texture_vert(f[i].iuv);
		}
		// calculate the normal. the direction of the triangle's face
		Vec3f normal = (world_pos[1]-world_pos[0])^(world_pos[2]-world_pos[0]);
		normal.normalize();
		// calculate the light level by dot product. the more parallel, the brighter
		// float light_level = normal.x*light_source.x + normal.y*light_source.y + normal.z*light_source.z;
		float light_level = normal*(Vec3f()-light_source);
		if (light_level<=0) continue;

        rasterize(world_pos, zbuffer, vt, model_uv, image, light_level, scale, camera_pos);
    }

	delete[] zbuffer;
}

// ----------------------------------------------------------------------
// ------------------ Other/Outdated Functions --------------------------
// ----------------------------------------------------------------------

// Draws line from one point to another using Besenham's Line Algorithm.
// Doesn't draw lines that go off screen
void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color) {
	// check if points are in bounds
    int w = image.get_width();
    int h = image.get_height();
	if (x0<0 or x0>w or x1<0 or x1>w or y0<0 or y0>h or y1<0 or y1>h) {
		std::cerr << "Renderer: line() called with point(s) out of image's bounds" << std::endl;
	}

	bool steep = false;
	// If it's steep, draw its inversion
	if (std::abs(y1-y0) > std::abs(x1-x0)) {
		steep = true;
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	// If points aren't left-to-right, make it so they are
	if (x1<x0) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1-x0;
    int dy = y1-y0;
    int slope2x = std::abs(dy)*2; // the slope * 2 * dx
    int error2x = 0;
    int y = y0;

	for (int x=x0; x<x1; x++) {

		// If inverted, uninvert it
		if (!steep) {
			image.set(x, y, color);
		}
		else {
			image.set(y, x, color);
		}

		error2x += slope2x;
        if (error2x>dx) {
            y += (y1>y0?1:-1);
            error2x -= 2*dx;
        } 
	}
}

void line(Vec2i v0, Vec2i v1, TGAImage& image, const TGAColor& color) {
	line(v0.x, v0.y, v1.x, v1.y, image, color);
}

// draw wireframe
void wireframe(Model *model, TGAImage& image, const TGAColor& color) {
    for (int i=0; i<model->nfaces(); i++) {
        std::vector<Vec3i> face = model->face(i);
		for (int j=0; j<3; j++) {
            Vec3f v0 = model->vert(face[j].ivert);
			Vec3f v1 = model->vert(face[(j+1)%3].ivert);
            
            int w = image.get_width();
            int h = image.get_height();
			int x0 = (v0.x+1)*w/2.;
			int y0 = (v0.y+1)*h/2.;
			int x1 = (v1.x+1)*w/2.;
			int y1 = (v1.y+1)*h/2.;

			line(x0, y0, x1, y1, image, color);
		}
	}
}


// simple triangle draw
void triangle(Vec3f screen_pos[], TGAImage& image, const TGAColor& color) {
	// find bounding box
	Vec2i bboxmin = Vec2i(image.get_width()-1, image.get_height()-1);
	Vec2i bboxmax = Vec2i(0, 0);
	for (int i=0; i<3; i++) {
		if (screen_pos[i].x < bboxmin.x) bboxmin.x = screen_pos[i].x;
		if (screen_pos[i].y < bboxmin.y) bboxmin.y = screen_pos[i].y;
		if (screen_pos[i].x > bboxmax.x) bboxmax.x = screen_pos[i].x;
		if (screen_pos[i].y > bboxmax.y) bboxmax.y = screen_pos[i].y;
	}
	
	// draw
	Vec2i P;
	for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
		for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
			Vec3f b = barycentric(screen_pos, P);
			const float EPS = 1e-5;
			if (b.x>=-EPS && b.y>=-EPS && b.z>=-EPS) {
				image.set(P.x, P.y, color);
			}
		}
	}
}