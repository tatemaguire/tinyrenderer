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
const int width  = 1000;
const int height = 1000;


int main(int argc, char** argv) {
	if (argc != 4) return 1;

	Model model = Model(argv[1], argv[2]);

	// create image
	TGAImage image = TGAImage(width, height, TGAImage::RGB);
	// render model
	render(model, image, Vec3f(0,0,-1), Vec3f(0,0,3));

	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.scale(width, height);
	image.write_tga_file(argv[3]);

	return 0;
}

