#pragma once

#include "Mesh.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace potato {

    enum class Token : unsigned char {
        Comment,
        Vertex,
        Face,
        Name,
        Normals,
        SmoothShading,
        Whitespace,
        Empty
    };

    Token parseSourceLine(const string &line);
    PolyMesh* loadOBJModel(string filename);

    Vert* parseVertex(const string &line);

    Face* parseFace(const string &line);
}; // namespace potato
