#pragma once

#include <memory>
#include <vector>
#include "Ray.hpp"
#include "Vector.hpp"
#include "interval.hpp"

namespace potato {

    class Material;

    template <typename T> class HitRecord {
      public:
        Vec3<T> position;
        Vec3<T> normal;
        double  t{};
        bool    frontFace{};
        shared_ptr<Material> material{};

        bool    setFaceNormal(const Ray &ray, const Vec3<T> outwardNormal) {
            // The outwardNormal is required to be of unit length.
            frontFace = ray.direction.dot(outwardNormal) < 0;
            if (frontFace) {
                this->normal = outwardNormal;
            } else {
                this->normal = outwardNormal * -1;
            }
            return this->frontFace;
        }
    };

    class object {
      public:
        Vec3d color{1.0, 1.0, 1.0};
        Vec3d position{};
        Vec3d emission{};
        object() = default;
        object(Vec3d position_, Vec3d emission_, Vec3d color_)
            : position(position_), emission(emission_), color(color_) {}
        virtual ~object() = default;

        virtual bool  intersect(const Ray         &ray,
                                Interval interval,
                                HitRecord<double> &rec) const = 0;
        virtual Vec3d getColor(const Ray &ray, const Vec3d &normal) const {
            return (normal + this->color) * 0.5;
        };
    };

    class AllObjects {
      public:
        std::vector<shared_ptr<object>> objects{};

        AllObjects() {}
        AllObjects(shared_ptr<object> obj){};

        void clear() { this->objects.clear(); }

        void add(shared_ptr<object> obj) { this->objects.push_back(obj); }
        shared_ptr<object> at(unsigned int i) { return this->objects.at(i); }

        bool intersect(const Ray &ray, Interval interval, HitRecord<double> &rec, shared_ptr<object> &hit) const {
            HitRecord<double> temp_rec;
            bool              hitAnything  = false;
            double            closestSoFar = interval.max;

            for (const auto &obj : this->objects) {
                if (obj->intersect(ray, Interval(interval.min, closestSoFar), temp_rec)) {
                    hitAnything  = true;
                    closestSoFar = temp_rec.t;
                    rec          = temp_rec;
                    hit = obj;
                }
            }
            return hitAnything;
        };
        virtual ~AllObjects() { this->clear(); };
    };
}; // namespace potato
