#include "PotatoRayEngine.hpp"
#include "Settings.hpp"
#include "interval.hpp"
#include "materials.hpp"
#include "objects.hpp"
#include "sphere.hpp"
#include <array>
#include <memory>

namespace potato {
    PotatoRayEngine::PotatoRayEngine(int windowWidth, int windowHeight)
        : PotatoRenderEngine(windowWidth, windowHeight) {

        auto ground = make_shared<Lambertian>(Vec3d(0.8,0.8,0.0));
        auto center = make_shared<Lambertian>(Vec3d(0.1,0.2,0.5));
        auto left = make_shared<Metal>(Vec3d(0.8,0.8,0.8));
        auto right = make_shared<Metal>(Vec3d(0.8,0.6,0.2));

        this->all.add(make_shared<Sphere>(Vec3d(0.0,-100.5, -1.0), zero, zero, 100.0, ground));
        this->all.add(make_shared<Sphere>(Vec3d(0.0,0.0, -1.2), zero, zero, 0.5, center));
        this->all.add(make_shared<Sphere>(Vec3d(-1.0,0.0, -1.0), zero, zero, 0.5, left));
        this->all.add(make_shared<Sphere>(Vec3d(1.0,0.0, -1.0), zero, zero, 0.5, right));
        //this->all.add(make_shared<Sphere>(Vec3d(0.0, 0.0, -1.0), zero, Vec3d(1.0,1.0,1.0), 0.5));
        //this->all.add(make_shared<Sphere>(Vec3d(0.0, -100.5, -1.0), zero, Vec3d(0.0, 1.0, 0.0), 100.0));

        this->camera = Camera(windowWidth, windowHeight);
        this->camera.initialize();
        // this->objects.push_back(
        // Sphere(Vec3d(windowWidth / 2.0, windowHeight / 2.0, -1.0), 100.0));
        // Sphere(Vec3d(windowWidth / 2.0, windowHeight / 2.0, 1.0), 100.0));
        //   Sphere(Vec3d(0.0, 0.0, -1.0), 0.5));
        /*
            this->aspectRatio    = float(windowWidth) / float(windowHeight);
            this->viewportHeight = 2.0;
            this->viewportWidth =
                this->viewportHeight * (double(windowWidth) / windowHeight);
            this->eyePoint          = Vec3d(0.0, 0.0, 0.0);
            this->focalLength       = 1.0f;
            this->viewportU         = Vec3f(this->viewportWidth, 0.0, 0.0);
            this->viewportV         = Vec3f(0.0, this->viewportHeight, 0.0);
            // this->pixelDeltaU       = this->viewportU / this->windowWidth;
            this->pixelDeltaU       = this->viewportU / this->windowWidth;
            // this->pixelDeltaV       = this->viewportV / this->windowHeight;
            this->pixelDeltaV       = this->viewportV / this->windowHeight;
            this->viewportUpperLeft = this->eyePoint -
                                      Vec3f(0.0, 0.0, this->focalLength) -
                                      this->viewportU / 2 - this->viewportV / 2;
            this->pixel00Loc = this->viewportUpperLeft +
                               (this->pixelDeltaU + this->pixelDeltaV) * 0.5;
            this->worldInterval = Interval(0, infinity);*/
    }

    void PotatoRayEngine::CameraShooter(Image<Vec3<float>> *buffer) {
        this->camera.render(this->all, buffer);
    }

    void PotatoRayEngine::ShootRays(const int &maxWidth, const int &maxHeight,
                                    Image<Vec3f> *buffer) {
        for (int x = 0; x < maxWidth; ++x) {
            for (int y = 0; y < maxHeight; ++y) {
                Vec3f pixelCenter = this->pixel00Loc + (this->pixelDeltaU * x) +
                                    (this->pixelDeltaV * y);

                Ray ray(this->eyePoint, pixelCenter - this->eyePoint);

                HitRecord<double>  closestRec;
                shared_ptr<object> closest;
                Interval           curInt =
                    Interval(this->worldInterval.min, this->worldInterval.max);
                for (int i = 0; i < this->all.objects.size(); ++i) {
                    HitRecord<double> temp_rec;
                    if (this->all.objects.at(i)->intersect(ray, curInt,
                                                           temp_rec)) {
                        closest    = this->all.objects.at(i);
                        curInt.max = temp_rec.t;
                        closestRec = temp_rec;
                    }
                }
                if (closest.get() != nullptr) {
                    buffer->setPixel(x, y,
                                     closest->getColor(ray, closestRec.normal));
                } else {
                    buffer->setPixel(x, y, getBackgroundColor(ray));
                }
            }
        }
    }

    Vec3d getBackgroundColor(const Ray &ray) {
        Vec3d normal = ray.direction.normalize();
        auto  a      = 0.5 * (normal.y + 1.0);
        return Vec3d(1.0, 1.0, 1.0) * (1.0 - a) + Vec3d(0.5, 0.7, 1.0) * a;
    }

    PotatoRayEngine::~PotatoRayEngine() {
        for (int i = 0; i < this->all.objects.size(); ++i) {
            // delete this->objects.at(i);
        }
        this->all.clear();
    }

    void PotatoRayEngine::renderToDrawBuffer(Image<Vec3f> *drawBuffer) {
        // this->ShootRays(this->windowWidth, this->windowHeight, drawBuffer);
        this->CameraShooter(drawBuffer);
    }
} // namespace potato
