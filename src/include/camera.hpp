#pragma once

#include "Buffer.hpp"
#include "Ray.hpp"
#include "objects.hpp"

namespace potato {
    class Camera {
      public:
        double aspectRatio     = 1.0;
        int    imageWidth      = 100;
        int    imageHeight     = 100;
        int    samplesPerPixel = 100;
        void   render(const object &obj);

        Camera(int imageWidth, int imageHeight)
            : imageWidth(imageWidth), imageHeight(imageHeight) {}

        void initialize() {
            this->aspectRatio    = float(imageWidth) / float(imageHeight);
            this->viewportHeight = 2.0;
            this->viewportWidth =
                this->viewportHeight * (double(imageWidth) / imageHeight);
            this->eyePoint          = Vec3d(0.0, 0.0, 0.0);
            this->focalLength       = 1.0f;
            this->viewportU         = Vec3f(this->viewportWidth, 0.0, 0.0);
            this->viewportV         = Vec3f(0.0, this->viewportHeight, 0.0);
            // this->pixelDeltaU       = this->viewportU / this->windowWidth;
            this->pixelDeltaU       = this->viewportU / this->imageWidth;
            // this->pixelDeltaV       = this->viewportV / this->windowHeight;
            this->pixelDeltaV       = this->viewportV / this->imageHeight;
            this->viewportUpperLeft = this->eyePoint -
                                      Vec3f(0.0, 0.0, this->focalLength) -
                                      this->viewportU / 2 - this->viewportV / 2;
            this->pixel00Loc = this->viewportUpperLeft +
                               (this->pixelDeltaU + this->pixelDeltaV) * 0.5;
            this->worldInterval = Interval(0, infinity);
            this->pixelSamplesScale = 1.0 / this->samplesPerPixel;
        };

        void  render(const AllObjects &obj, Image<Vec3f> *buffer);

        Vec3d getRealColor(const Vec3d &color);

      private:
        double pixelSamplesScale;
        Vec3d    center;
        float    viewportHeight{};
        float    viewportWidth{};
        Vec3d    eyePoint{};
        float    focalLength{};
        Vec3f    viewportU{};
        Vec3f    viewportV{};
        Vec3d    pixelDeltaU{};
        Vec3d    pixelDeltaV{};
        Vec3f    viewportUpperLeft{};
        Vec3f    pixel00Loc{};
        Interval worldInterval{-infinity, infinity};

        Vec3d    rayColor(const Ray &ray, const AllObjects &obj) const;
        Vec3d    sampleSquare() const;
        Ray      getRay(int x, int y) const;
    };
}; // namespace potato
