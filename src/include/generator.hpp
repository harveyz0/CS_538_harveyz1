#pragma once

#include "Vector.hpp"

#include "Mesh.hpp"

using namespace std;

namespace potato {
    PolyMesh *generateTestFan(const Vec3f &center, float radius,
                              int triangleCnt);
    PolyMesh *generateConvexPolygon(const Vec3f &center, float radius,
                                    int triangleCnt);
    PolyMesh *generateConcavePolygon(const Vec3f &center, float radius,
                                     int triangleCnt);
}; // namespace potato
