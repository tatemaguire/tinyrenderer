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
Model *model = NULL;
const int width  = 800;
const int height = 800;


int main(int argc, char** argv) {
	if (argc == 2) {
		model = new Model(argv[1]);
	} else {
		model = new Model("./obj/african_head.obj");
	}

	TGAImage image = TGAImage(width, height, TGAImage::RGB);

	// Vec2i pts[3] = {Vec2i(10,10), Vec2i(100, 30), Vec2i(190, 160)}; 
    // triangle(pts, image, green); 

	tri_render_test(model, image);

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	// image.scale(image.get_width()*10, image.get_height()*10);
	image.write_tga_file("output.tga");

	delete model;
	return 0;
}

