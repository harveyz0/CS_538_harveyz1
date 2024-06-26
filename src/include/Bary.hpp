#pragma once

#include <cmath>
#include <vector>
#include <iostream>
#include "Vector.hpp"
#include "Line.hpp"
#include "Mesh.hpp"
using namespace std;

namespace potato {       
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
        BaryData(Vert &A, Vert &B, Vert &C) {
            AB = ImplicitLine<float>(A.pos,B.pos);
            AC = ImplicitLine<float>(A.pos,C.pos);
            Cval = AB.eval(C.pos.x, C.pos.y);
            Bval = AC.eval(B.pos.x, B.pos.y);
        };
    };
    
    inline Vec3f barycentric(   BaryData &bd,                    
                                float x, float y) {
        Vec3f bary;
        bary.z = bd.AB.eval(x, y) / bd.Cval;
        bary.y = bd.AC.eval(x, y) / bd.Bval;
        bary.x = 1.0f - bary.y - bary.z;
        return bary;
    };    
};
