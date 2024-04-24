#pragma once

#include "objects.hpp"
#include <memory>

namespace potato {
    class Sphere : public object {
      public:
        double               radius;
        shared_ptr<Material> material;

        Sphere() = default;
        Sphere(Vec3d position, Vec3d emission, Vec3d color, double radius)
            : object(position, emission, color), radius(radius) {}
        Sphere(Vec3d position, Vec3d emission, Vec3d color, double radius,
               shared_ptr<Material> material)
            : object(position, emission, color), radius(radius),
              material(material) {}
        /*Sphere(Vec3d position, double radius)
            : object(position, Vec3d(0.0, 0.0, 0.0), Vec3d(1.0, 1.0, 1.0)),
              radius(radius) {}
        Sphere(Vec3d position, double radius, Vec3d color)
            : object(position, Vec3d(0.0, 0.0, 0.0), color), radius(radius) {}*/
        virtual ~Sphere() = default;
        double       intersection(const Ray &ray) const;
        virtual bool intersect(const Ray &ray, Interval interval,
                               HitRecord<double> &rec) const override {
            Vec3d  OC  = this->position - ray.origin;
            double a   = ray.direction.length_squared();
            double h   = ray.direction.dot(OC);
            double c   = OC.length_squared() - (this->radius * this->radius);
            double dis = (h * h) - (a * c);

            if (dis < 0)
                return false;

            double sqrtd = sqrt(dis);

            double root  = (h - sqrtd) / a;
            if (root <= interval.min || root >= interval.max) {
                root = (h + sqrtd) / a;
                if (root <= interval.min || interval.max <= root) {
                    return false;
                }
            }

            rec.t        = root;
            rec.position = ray.intersectPoint(rec.t);
            rec.setFaceNormal(ray,
                              (rec.position - this->position) / this->radius);
            rec.material = this->material;

            return true;
        };

        virtual Vec3d getColor(const Ray   &ray,
                               const Vec3d &normal) const override {
            return this->color;
        };
    };

}; // namespace potato
