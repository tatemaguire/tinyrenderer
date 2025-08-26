// Author: Tate Maguire
// August 25, 2025

#include <iostream>
#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "geometry.h"
#include "model.h"
#include "renderer.h"

// Globals
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   0  );
const TGAColor blue  = TGAColor(0,   0,   255, 0  );
Model *model = NULL;
const int width  = 1000;
const int height = 1000;


int main(int argc, char** argv) {
	if (argc == 2) {
		model = new Model(argv[1]);
	} else {
		model = new Model("./obj/african_head.obj");
	}

	TGAImage image = TGAImage(width, height, TGAImage::RGB);

	// Vec2i pts[3] = {Vec2i(10,10), Vec2i(100, 30), Vec2i(190, 160)}; 
    // triangle(pts, image, green); 

	Vec3f light_source = Vec3f(0, 0, -1);
	tri_render_light(model, image, light_source);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.scale(width, height);
	image.write_tga_file("output.tga");

	delete model;
	return 0;
}

