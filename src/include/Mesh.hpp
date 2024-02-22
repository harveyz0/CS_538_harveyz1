#pragma once

#include <cmath>
#include <vector>
#include <iostream>
#include "Line.hpp"
#include "Vector.hpp"
using namespace std;

namespace potato {

    struct Vert {
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

    template<typename T>
    struct BoundBox {
        Vec3<T> start;
        Vec3<T> end;
    };

    template<typename T>
    Vec3<T> min(Vec3<T> a, Vec3<T> b) {
        Vec3<T> c;
        c.x = std::min(a.x, b.x);
        c.y = std::min(a.y, b.y);
        c.z = std::min(a.z, b.z);
        return c;
    };

    template<typename T>
    Vec3<T> max(Vec3<T> a, Vec3<T> b) {
        Vec3<T> c;
        c.x = std::max(a.x, b.x);
        c.y = std::max(a.y, b.y);
        c.z = std::max(a.z, b.z);
        return c;
    };

    template<typename T>
    BoundBox<T> computeBounds(Vec3<T> A, Vec3<T> B, Vec3<T> C) {
        BoundBox<T> box;
        box.start = min(min(A,B),C);
        box.end = max(max(A,B),C);        
        return box;
    };

    template<typename T, typename C>
    BoundBox<T> computeBounds(Polygon &poly) {
        BoundBox<T> box;
        box.start = poly.vertices.at(0);
        box.end = poly.vertices.at(0);
        for(int i = 1; i < poly.vertices.size(); i++) {
            Vec3<T> v = poly.vertices.at(i).pos;
            box.start = min(box.start, v);
            box.end = max(box.end, v);
        }
        return box;
    };

    struct BaryData {
        ImplicitLine<float> AB;
        ImplicitLine<float> AC;
        float Cval {};
        float Bval {};

        BaryData(Vec3f &A, Vec3f &B, Vec3f &C) {
            AB = ImplicitLine<float>(A,B);
            AC = ImplicitLine<float>(A,C);
            Cval = AB.eval(C.x, C.y);
            Bval = AC.eval(B.x, B.y);
        };
    };
    
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
