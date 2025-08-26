// Author: Tate Maguire
// August 25, 2025

#include <iostream>
#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"
#include "renderer.h"

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

// Gets barycentric coordinates of P within the triangle defined by pts
// pts must have length 3
Vec3f barycentric(Vec2i* pts, Vec2i P) {
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

void triangle(Vec2i pts[], TGAImage& image, const TGAColor& color) {
	// find bounding box
	Vec2i bboxmin = Vec2i(image.get_width()-1, image.get_height()-1);
	Vec2i bboxmax = Vec2i(0, 0);
	for (int i=0; i<3; i++) {
		if (pts[i].x < bboxmin.x) bboxmin.x = pts[i].x;
		if (pts[i].y < bboxmin.y) bboxmin.y = pts[i].y;
		if (pts[i].x > bboxmax.x) bboxmax.x = pts[i].x;
		if (pts[i].y > bboxmax.y) bboxmax.y = pts[i].y;
	}
	
	// draw
	Vec2i P;
	for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
		for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
			Vec3f b = barycentric(pts, P);
			const float EPS = 1e-5;
			if (b.x>=-EPS && b.y>=-EPS && b.z>=-EPS) {
				image.set(P.x, P.y, color);
			}
		}
	}
}

void wireframe(Model *model, TGAImage& image, const TGAColor& color) {
    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> face = model->face(i);
		for (int j=0; j<3; j++) {
            Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j+1)%3]);
            
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

void tri_render_test(Model* model, TGAImage& image) {
    for (int i=0; i<model->nfaces(); i++) {
        std::vector<int> f = model->face(i);
        Vec2i pts[3];
        for (int j=0; j<3; j++) {
            Vec3f v = model->vert(f[j]);
            v.x = (v.x+1.)*image.get_width()/2;
            v.y = (v.y+1.)*image.get_height()/2;
            pts[j] = Vec2i(v.x, v.y);
        }
        TGAColor random_color = TGAColor(rand()%255, rand()%255, rand()%255, 255);
        triangle(pts, image, random_color);
    }
}
