// Author: Dimitry V. Sokolov (I think?)
// Modified by: Tate Maguire

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "model.h"

Model::Model(const char *filename) : verts_(), texture_verts_(), faces_() {
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0;i<3;i++) iss >> v.raw[i];
            verts_.push_back(v);
        } else if (!line.compare(0, 3, "vt ")) {
            Vec2f vt;
            iss >> trash >> trash; // for some reason there's two spaces after vt
            for (int i=0; i<2; i++) iss >> vt.raw[i];
            texture_verts_.push_back(vt);
        } else if (!line.compare(0, 2, "f ")) {
            std::vector<Vec3i> f;
            int ivert, iuv, inorm;
            iss >> trash;
            while (iss >> ivert >> trash >> iuv >> trash >> inorm) {
                // in wavefront obj all indices start at 1, not zero
                f.push_back(Vec3i(--ivert, --iuv, --inorm));
            }
            std::swap(f[1].iuv, f[2].iuv); // idk why but this is necessary for textures to work
            faces_.push_back(f);
        }
    }
    std::cerr << "# v# " << verts_.size() << " f# "  << faces_.size() << std::endl;
}

Model::~Model() {
}

int Model::nverts() {
    return (int)verts_.size();
}

int Model::ntexture_verts() {
    return (int)texture_verts_.size();
}

int Model::nfaces() {
    return (int)faces_.size();
}

Vec3f Model::vert(int i) {
    return verts_[i];
}

Vec2f Model::texture_vert(int i) {
    return texture_verts_[i];
}

std::vector<Vec3i> Model::face(int idx) {
    return faces_[idx];
}
