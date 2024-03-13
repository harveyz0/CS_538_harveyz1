#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>
#include "Vector.hpp"
#include "Object3D.hpp"
#include "Fragment.hpp"
#include "BoundBox.hpp"
#include "Mesh.hpp"
#include "Bary.hpp"
#include "Settings.hpp"
using namespace std;

namespace potato {
    // Already provided
    PolyMesh* generateTestFan(const Vec3f &center, float radius, int triangleCnt);
    void drawLines(PolyMesh *mesh, vector<Fragment> &fragList, bool wireframe);
    void fillTriangles(PolyMesh *mesh, vector<Fragment> &fragList);    
    Fragment computeFragment(Vert &vA, Vert &vB, Vert &vC, Vec3f &bc);  

    // TODO FOR ASSIGNMENT    
    void drawLineDDA(Vert &startVert, Vert &endVert, vector<Fragment> &fragList, bool wireframe);
    void drawLineMid(Vert &startVert, Vert &endVert, vector<Fragment> &fragList, bool wireframe);    

template<typename T>
inline float
calculateMidpoint(float x, float y, Vec3<T> start, Vec3<T> end)
// This should probably be fixed to allow doubles
{
  return (start.y - end.y) * x + (end.x - start.x) * y + (start.x * end.y) -
         (end.x * start.y);
};


    void fillTriangle(vector<Vert> &vertices, Face &face, vector<Fragment> &fragList);   
    
};
