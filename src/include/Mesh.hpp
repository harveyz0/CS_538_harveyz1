#pragma once

#include <cmath>
#include <vector>
#include <iostream>
#include "Line.hpp"
#include "Vector.hpp"
using namespace std;

namespace potato {

    template<typename T, typename C>
    struct Vert {
        Vec3<T> pos;
        Vec3<C> color;
    };

    template<typename T, typename C>
    struct Polygon {
        vector<Vert<T,C>> vertices;
    };

    template<typename T, typename C>
    struct TriangleMesh {
        vector<Vert<T,C>> vertices;
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
    BoundBox<T> computeBounds(Polygon<T,C> &poly) {
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

    template<typename T>
    Vec3<float> barycentric(Vec3<T> A,
                            Vec3<T> B,
                            Vec3<T> C,
                            float x, float y) {
        Vec3<float> bary;
        //Line<T,S> lineAB;
        //lineAB.start = A;
        //lineAB.end = B;

        //Line<T,S> lineAC;
        //lineAC.start = A;
        //lineAC.end = C;

        bary.z = implicit(A,B, x, y) / implicit(A,B, C.x, C.y);
        bary.y = implicit(A,C, x, y) / implicit(A,C, B.x, B.y);
        bary.x = 1.0 - bary.y - bary.z;

        return bary;
    };

    template<typename T, typename S>
    void fillTriangle(  Image<Vec3<S>> *image,
                        Vec3<T> A, Vec3<T> B, Vec3<T> C) {
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
    };
};
