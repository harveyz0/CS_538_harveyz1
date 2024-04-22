#pragma once

#include "Vector.hpp"
#include "Settings.hpp"
#include "interval.hpp"

namespace potato {
    class Ray {
      public:
        Vec3d origin{};
        Vec3d direction{};
       // double tMin{0.0};
       // double tMax{infinity};

        Ray(Vec3d origin_, Vec3d direction_)
            : origin(origin_), direction(direction_) {}

        Vec3d intersectPoint(double t) const {
            return this->origin + this->direction * t;
        }
    };
}; // namespace potato
