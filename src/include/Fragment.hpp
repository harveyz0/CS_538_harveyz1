#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <iostream>
#include "Vector.hpp"
using namespace std;

namespace potato {
    struct Fragment {
        Vec3i pos {};        
        Vec4f color {};

    Fragment() = default;
  Fragment(int x, int y, int z, float r, float g, float b, float a)
    : pos(Vec3i(x, y, z))
    , color(Vec4f(r, g, b, a)){};

  Fragment(int x, int y, Vec4f color)
    : pos{ Vec3i(x, y, 0) }
    , color{ color } {};
  Fragment(int x, int y, Vec3f color)
    : pos{ Vec3i(x, y, 0) }
    , color{ Vec4f(color, 0) } {};
    };
};
