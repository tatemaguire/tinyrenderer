// Author: Tate Maguire
// August 25, 2025

#ifndef TATE_RENDERER_H
#define TATE_RENDERER_H

#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);
void line(Vec2i v0, Vec2i v1, TGAImage& image, const TGAColor& color);
Vec3f barycentric(Vec3f* pts, Vec2i P);
void triangle(Vec3f pts[], int* zbuffer, Vec2f vt[], TGAImage& model_uv, TGAImage& image, float light_level);
void rasterize(Vec3f pts[], int* zbuffer, Vec2f vt[], TGAImage& model_uv, TGAImage& image, float light_level);

void wireframe(Model* model, TGAImage& image, const TGAColor& color);
void render(Model* model, TGAImage& model_uv, TGAImage& image, Vec3f light_source);

#endif // TATE_RENDERER_MAIN_H