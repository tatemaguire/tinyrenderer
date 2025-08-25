#include <iostream>
#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
Model *model = NULL;
const int width  = 1000;
const int height = 1000;

void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color) {
	// check if points are in bounds
	if (x0<0 or x0>width or x1<0 or x1>width or y0<0 or y0>height or y1<0 or y1>height) {
		std::cerr << "line out of bounds" << std::endl;
		return;
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

void wireframe(Model *model, TGAImage& image, const TGAColor& color) {
	for (int i=0; i<model->nfaces(); i++) {
		std::vector<int> face = model->face(i);
		for (int j=0; j<3; j++) {
			Vec3f v0 = model->vert(face[j]);
			Vec3f v1 = model->vert(face[(j+1)%3]);

			int x0 = (v0.x+1)*width/2.;
			int y0 = (v0.y+1)*height/2.;
			int x1 = (v1.x+1)*width/2.;
			int y1 = (v1.y+1)*height/2.;

			line(x0, y0, x1, y1, image, color);
		}
	}
}

int main(int argc, char** argv) {
	if (argc == 2) {
		model = new Model(argv[1]);
	} else {
		model = new Model("./obj/african_head.obj");
	}

	TGAImage image(width, height, TGAImage::RGB);

	wireframe(model, image, white);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	// image.scale(image.get_width()*10, image.get_height()*10);
	image.write_tga_file("output.tga");

	delete model;
	return 0;
}

