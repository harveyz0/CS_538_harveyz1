
#include "objects.hpp"

namespace potato {
    // std::array<double, 2> Sphere::intersection(const Ray &ray) {
    double Sphere::intersection(const Ray &ray) const {

        Vec3d  OC  = this->position - ray.origin;
        double a   = ray.direction.length_squared();
        // double a   = ray.direction.dot(ray.direction);
        double h   = ray.direction.dot(OC);
        // double b   = ray.direction.dot(OC) * -2.0;
        double c   = OC.length_squared() - (this->radius * this->radius);

        double dis = (h * h) - (a * c);
        if (dis < 0) {
            return -1.0;
        }
        return (h - sqrt(dis)) / a;
    };

    bool Sphere::intersect(const Ray &ray, Interval interval, HitRecord<double> &rec) const {
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
        rec.setFaceNormal(ray, (rec.position - this->position) / this->radius);

        return true;
    };

    Vec3d Sphere::getColor(const Ray &ray, const Vec3d &normal) const {
    return this->color;
     /*   return Vec3d((normal.x + this->color.x) * 0.5, 
                     (normal.y + this->color.y) * 0.5,
                     (normal.z + this->color.z) * 0.5);*/
    };
}; // namespace potato
