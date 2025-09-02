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
Model *model = NULL;
TGAImage model_uv;
const int width  = 1000;
const int height = 1000;


int main(int argc, char** argv) {
	if (argc >= 2) {
		model = new Model(argv[1]);
	} else {
		model = new Model("obj/african_head/african_head.obj");
	}
	if (argc >= 3) {
		model_uv.read_tga_file(argv[2]);
	} else {
		model_uv.read_tga_file("obj/african_head/african_head_diffuse.tga");
	}
	model_uv.flip_vertically();

	// create image
	TGAImage image = TGAImage(width, height, TGAImage::RGB);
	// render model
	render(model, model_uv, image, Vec3f(0,0,-1), Vec3f(0,0,3));

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.scale(width, height);
	image.write_tga_file("output.tga");

	delete model;
	return 0;
}

