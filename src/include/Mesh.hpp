#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>
#include "Vector.hpp"
==== BASE ====
==== BASE ====
using namespace std;

namespace potato {

    struct Vert {
==== BASE ====
        Vec3f pos;
        Vec4f color; // [0,1]
    };

    struct Polygon {
        vector<Vert> vertices;
    };
    
    struct TriangleMesh {
        vector<Vert> vertices;
        vector<unsigned int> indices;
    };
==== BASE ====

        Vert operator+(const Vert &other) const {
            return {
                pos + other.pos,
                color + other.color
            };
        };

        Vert operator-(const Vert &other) const {
            return {
                pos - other.pos,
                color - other.color
            };
        };

        Vert operator*(const Vert &other) const {
            return {
                pos * other.pos,
                color * other.color
            };
        };

==== BASE ====
    template<typename T>
    BoundBox<T> computeBounds(Vec3<T> A, Vec3<T> B, Vec3<T> C) {
        BoundBox<T> box;
        box.start = min(min(A,B),C);
        box.end = max(max(A,B),C);        
        return box;
==== BASE ====
    };

    struct Face {
        vector<unsigned int> indices {};
    };
        
    class PolyMesh : public Object3D {
    protected:
        vector<Vert> vertices {};
        vector<Face> faces {};
    public:
        PolyMesh() : Object3D() {};
        virtual ~PolyMesh() {};

        vector<Vert>& getVertices() { return vertices; };
        vector<Face>& getFaces() { return faces; };
    };
==== BASE ====
    
    inline Vec3<float> barycentric(BaryData &bd,                    
                            float x, float y) {
        Vec3<float> bary;
        bary.z = bd.AB.eval(x, y) / bd.Cval;
        bary.y = bd.AC.eval(x, y) / bd.Bval;
        bary.x = 1.0 - bary.y - bary.z;
        return bary;
    };
    
    inline void calculateTriangleBarycentric(  
                                        Vec3f &A,
                                        Vec3f &B,
                                        Vec3f &C,
                                        vector<Vec3f> &allBary) {

        BoundBox bb = computeBounds(A,B,C);
        int sy = floor(bb.start.y);
        int ey = floor(bb.end.y);
        int sx = floor(bb.start.x);
        int ex = floor(bb.end.x);
        BaryData bd(A,B,C);
        
        for(int y = sy; y <= ey; y++) {
            for(int x = sx; x <= ex; x++) {
                Vec3<float> bary = barycentric(bd,float(x),float(y));
                if(bary.x > 0 && bary.y > 0 && bary.z > 0) {
                    allBary.push_back(bary);
                }
            }
        }
    };
==== BASE ====
// Compute bounds for ENTIRE mesh
    void computeBounds(PolyMesh *mesh, BoundBoxf &box, bool startBox=true);

    /*
    void fillTriangle(  Image<Vec3f> *image,
                        Vec3f A, Vec3f B, Vec3f C) {
        BoundBox bb = computeBounds(A,B,C);
        for(auto y = bb.start.y; y <= bb.end.y; y += 1) {
            for(auto x = bb.start.x; x <= bb.end.x; x += 1) {
                Vec3<float> bary = barycentric(A,B,C,float(x),float(y));
                if(bary.x > 0 && bary.y > 0 && bary.z > 0) {
                    Vec3<S> color = Vec3<S>(
                        static_cast<S>(255.0*bary.x),
                        static_cast<S>(255.0*bary.y),
                        static_cast<S>(255.0*bary.z)
                    );
                    image->setPixel(static_cast<int>(x),
                                    static_cast<int>(y),
                                    color);
                }
            }
        }
    };*/
};
