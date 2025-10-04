// Author: Tate Maguire
// August 25, 2025

#ifndef TATE_RENDERER_H
#define TATE_RENDERER_H

#include "tgaimage.h"
#include "geometry.h"
#include "model.h"

class RenderEnvironment {
    private:
        Matrix<float> transform_matrix; // 4 by 4
        Vec3f global_light;
    public:
        RenderEnvironment(Vec3f global_light);
        // TODO: finish this class
};

Vec3f barycentric(Vec3f* pts, Vec2i P);
void triangle(Vec3f pts[], int* zbuffer, Vec2f vt[], TGAImage& model_uv, TGAImage& image, float light_level);
void rasterize(Vec3f pts[], int* zbuffer, Vec2f vt[], TGAImage& model_uv, TGAImage& image, float light_level, float scale, Vec3f camera_pos);
void render(Model& model, TGAImage& image, Vec3f light_source, Vec3f camera_pos);

void rasterize(std::vector<Vec3i>& face, Model& model, Matrix<float>& transform_matrix); //TODO: finish alternate def

// void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);
// void line(Vec2i v0, Vec2i v1, TGAImage& image, const TGAColor& color);
// void wireframe(Model* model, TGAImage& image, const TGAColor& color);
// void triangle(Vec3f screen_pos[], TGAImage& image, const TGAColor& color);

#endif // TATE_RENDERER_MAIN_H