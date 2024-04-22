
#include "Buffer.hpp"
#include "Settings.hpp"
#include "camera.hpp"
#include "interval.hpp"
#include "objects.hpp"
#include <memory>

namespace potato {

    Vec3d Camera::rayColor(const Ray &ray, const AllObjects &obj) const {
        HitRecord<double>  rec;
        shared_ptr<object> hit;
        if (obj.intersect(ray, Interval(0, infinity), rec, hit)) {
            return hit->getColor(ray, rec.normal);
        }
        Vec3d normal = ray.direction.normalize();
        auto  a      = 0.5 * (normal.y + 1.0);
        return Vec3d(1.0, 1.0, 1.0) * (1.0 - a) + Vec3d(0.5, 0.7, 1.0) * a;
    };

    void Camera::render(const AllObjects &obj, Image<Vec3f> *buffer) {

        static const Interval intensity(0.000, 0.999);
        for (int y = 0; y < this->imageHeight; ++y) {
            for (int x = 0; x < this->imageWidth; ++x) {
                Vec3d pixelColor(0.0, 0.0, 0.0);
                for (int sample = 0; sample < this->samplesPerPixel; ++sample) {
                    Ray r      = this->getRay(x, y);
                    pixelColor = pixelColor + this->rayColor(r, obj);
                }
                buffer->setPixel(x, y, this->rayColor(this->getRay(x, y), obj));
            }
        }
    };

    Ray Camera::getRay(int x, int y) const {
        Vec3d offset      = this->sampleSquare();
        auto  pixelCenter = this->pixel00Loc +
                           (this->pixelDeltaU * (offset.x + x)) +
                           (this->pixelDeltaV * (offset.y + y));
        return Ray(this->eyePoint, pixelCenter - this->eyePoint);
    };

    Vec3d Camera::sampleSquare() const {
        return Vec3d(random_double() - 0.5, random_double() - 0.5, 0.0);
    }

}; // namespace potato
