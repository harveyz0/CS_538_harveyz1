#pragma once

#include "PotatoRenderEngine.hpp"
#include "Settings.hpp"
#include "camera.hpp"
#include "objects.hpp"
#include "sphere.hpp"

namespace potato {
    class PotatoRayEngine : public PotatoRenderEngine {
      private:
        float aspectRatio{};
        float viewportHeight{};
        float viewportWidth{};
        Vec3d eyePoint{};
        float focalLength{};
        Vec3f viewportU{};
        Vec3f viewportV{};
        Vec3d pixelDeltaU{};
        Vec3d pixelDeltaV{};
        Vec3f viewportUpperLeft{};
        Vec3f pixel00Loc{};
        Interval worldInterval{-infinity, infinity};
        Camera camera; //{100,100};
      public:
        //vector<Sphere> objects{};
        AllObjects all{};
        PotatoRayEngine(int windowWidth, int windowHeight);
        virtual ~PotatoRayEngine();
        void         ShootRays(const int &maxWidth, const int &maxHeight,
                               Image<Vec3f> *buffer);
        virtual void initialize() override; 
        virtual void renderToDrawBuffer(Image<Vec3f> *drawBuffer);
        void CameraShooter(Image<Vec3<float>> *buffer);
    };

    Vec3d getBackgroundColor(const Ray &ray);
}; // namespace potato
