#pragma once

#include "Buffer.hpp"
#include "Ray.hpp"
#include "Settings.hpp"
#include "objects.hpp"
#include <mutex>
#include <numeric>
#include <string>
#include <thread>
#include <vector>

namespace potato {
    class Camera {
      public:
        double aspectRatio               = 1.0;
        int    imageWidth                = 100;
        int    imageHeight               = 100;
        // This specifies how many ray we shoot out per pixel;
        int samplesPerPixel              = 50;
        // This will tell us how many bounces a ray can have;
        int                   maxDepth   = 10;
        //These two are only here so that the threads can get at them
        Image<Vec3<float>>   *buffer     = nullptr;
        AllObjects           *allObjects = nullptr;
        void                  render(const object &obj);

        vector<std::thread *> workers;
        vector<int>           rowsLeft;
        mutex                *rowsLock;

        std::thread          *manager   = nullptr;

        bool                  frameDone = false;

        Camera() {};
        Camera(int imageWidth, int imageHeight)
            : imageWidth(imageWidth), imageHeight(imageHeight) {
            cout << "Making new camera " << endl;
        }
        ~Camera() {
            cout << "Destroying camera " << endl;
            this->frameDone = true;

            for (thread *t : this->workers) {
                t->join();

                delete t;
            }
            this->workers.clear();
            if (this->manager != nullptr) {
                this->manager->join();
                delete this->manager;
            }
            delete this->rowsLock;
            this->rowsLeft.clear();
        };

        void initialize() {
            this->rowsLock       = new mutex();
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
            this->worldInterval     = Interval(0, infinity);
            this->pixelSamplesScale = 1.0 / this->samplesPerPixel;

            this->RefillRowsLeft();
        };

        void RefillRowsLeft() {
            this->rowsLeft.clear();
            this->rowsLeft.reserve(this->imageHeight);
            // std::iota(this->rowsLeft.begin(), this->rowsLeft.end(), 0);
            for (int i = 0; i < this->imageHeight; ++i) {
                this->rowsLeft.push_back(i);
            }
            // cout << this->rowsLeft << " POOOOOOPSSS" << endl;
        };

        void StartThreads() {
            // if(this->manager != nullptr){ return;}
            this->manager =
                new std::thread(&Camera::MonitorFrameCompletion, this);
            int currentTop = 0;
            int perThread  = this->imageHeight / numThreads;
            for (int i = 0; i < numThreads - 1; ++i) {
                workers.push_back(new std::thread(&Camera::RunWorker, this,
                                                  currentTop,
                                                  currentTop + perThread));
                currentTop += perThread;
            }
            workers.push_back(new std::thread(&Camera::RunWorker, this,
                                              currentTop, this->imageHeight));
            cout << "Made some threads " << workers.size() << endl;
        };

        void  render(const AllObjects &obj, Image<Vec3f> *buffer);

        Vec3d getRealColor(const Vec3d &color);

      private:
        // I don't think we need this value as it seems like all it does is
        // makes the image darker.
        // I think it has something to do with gamma correction but I don't
        // think that's a problem we have with this path tracer.
        double   pixelSamplesScale;
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

        // Vec3d    rayColor(const Ray &ray, const AllObjects &obj) const;
        Vec3d rayColor(const Ray &ray, int maxDepth,
                       const AllObjects &obj) const;
        Vec3d sampleSquare() const;
        Ray   getRay(int x, int y) const;
        void  DoColumn(int y);
        void  RunWorker(int beginningY, int endingY);
        void  MonitorFrameCompletion();
    };
}; // namespace potato
