#pragma once

#include "Vector.hpp"
#include "objects.hpp"
namespace potato {
// A lot of this file comes from https://raytracing.github.io/books/RayTracingInOneWeekend.html In the metal section

    class Material {
      public:
        virtual ~Material() = default;

        virtual bool scatter(const Ray &rayIn, const HitRecord<double> &rec,
                             Vec3d &attenuation, Ray &scattered) const {
            return false;
        };
    };

    class Lambertian : public Material {
      public:
        Lambertian(const Vec3d &color) : albedo(color) {}
        bool scatter(const Ray &rayIn, const HitRecord<double> &rec,
                     Vec3d &attenuation, Ray &scattered) const override {
            auto scatter_direction = rec.normal + randomNormalizeVec3();
            if (scatter_direction.nearZero())
                scatter_direction = rec.normal;
            scattered   = Ray(rec.position, scatter_direction);
            attenuation = this->albedo;
            return true;
        }

      private:
        Vec3d albedo;
    };

    class Metal : public Material {
      public:
        Metal(const Vec3d &color) : albedo(color) {}

        bool scatter(const Ray &rayIn, const HitRecord<double> &rec,
                     Vec3d &attenuation, Ray &scattered) const override {
            Vec3d reflected = reflect(rayIn.direction, rec.normal);
            scattered       = Ray(rec.position, reflected);
            attenuation     = albedo;
            return true;
        };

      private:
        Vec3d albedo;
    };
}; // namespace potato
