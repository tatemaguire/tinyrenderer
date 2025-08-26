// Author: Tate Maguire
// August 25, 2025

#ifndef TATE_RENDERER_H
#define TATE_RENDERER_H

#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);
void line(Vec2i v0, Vec2i v1, TGAImage& image, const TGAColor& color);
Vec3f barycentric(Vec2i* pts, Vec2i P);
void triangle(Vec2i pts[], TGAImage& image, const TGAColor& color);

void wireframe(Model* model, TGAImage& image, const TGAColor& color);
void tri_render_test(Model* model, TGAImage& image);

#endif // TATE_RENDERER_MAIN_H