
#include "Buffer.hpp"
#include "Settings.hpp"
#include "Vector.hpp"
#include "camera.hpp"
#include "interval.hpp"
#include "materials.hpp"
#include "objects.hpp"
#include <memory>
#include <thread>

namespace potato {

    Vec3d Camera::rayColor(const Ray &ray, int depth,
                           const AllObjects &obj) const {
        if (depth <= 0)
            return Vec3d(0.0, 0.0, 0.0);

        HitRecord<double>  rec;
        shared_ptr<object> hit;
        if (obj.intersect(ray, Interval(0.001, infinity), rec, hit)) {
            Ray   scattered;
            Vec3d attenuation;
            if (rec.material->scatter(ray, rec, attenuation, scattered))
                return attenuation * this->rayColor(scattered, depth - 1, obj);
            /*return this->rayColor(
                       Ray(ray.origin,
                           rec.normal + randomOnHemisphere(rec.normal)),
                       depth - 1, obj) *
                   0.9;*/
            return zero;
            // return hit->getColor(ray, rec.normal) * 0.5;
        }
        Vec3d normal = ray.direction.normalize();
        auto  a      = 0.5 * (normal.y + 1.0);
        return Vec3d(1.0, 1.0, 1.0) * (1.0 - a) + Vec3d(0.5, 0.7, 1.0) * a;
    };

    void Camera::DoColumn(int y) {

        for (int x = 0; x < this->imageWidth; ++x) {
            Vec3d pixelColor(0.0, 0.0, 0.0);
            for (int sample = 0; sample < this->samplesPerPixel; ++sample) {
                Ray r      = this->getRay(x, y);
                pixelColor = pixelColor + this->rayColor(r, this->maxDepth,
                                                         *this->allObjects);
            }
            this->buffer->setPixel(x, y,
                                   this->rayColor(this->getRay(x, y),
                                                  this->maxDepth,
                                                  *this->allObjects));
        }
    };

    void Camera::RunWorker(int beginningY, int endingY) {
        bool areWeDone = false;
        while (!areWeDone) {
            this_thread::sleep_for(chrono::milliseconds(15));
            if (this->rowsLeft.size() == 0) {
                continue;
            }
        this->rowsLock->lock();
            int currentRow = this->rowsLeft.at(this->rowsLeft.size() - 1);
            this->rowsLeft.pop_back();
        this->rowsLock->unlock();
        //for(int y = beginningY; y < endingY; ++y)
            this->DoColumn(currentRow);
        }
    };

    void Camera::MonitorFrameCompletion() {
        while (true) {
            this_thread::sleep_for(chrono::milliseconds(100));
            if (this->rowsLeft.size() == 0) {
                this->RefillRowsLeft();
            }
        }
    };

    void Camera::render(const AllObjects &obj, Image<Vec3f> *buffer) {
        static const Interval intensity(0.000, 0.999);
        vector<std::thread>   currentThreads;

        for (int y = 0; y < this->imageHeight; ++y) {
            currentThreads.push_back(std::thread(&Camera::DoColumn, this, y));
            this_thread::sleep_for(chrono::milliseconds(15));
            if ((y % numThreads) == 0) {
                while (currentThreads.size() != 0) {
                    this_thread::sleep_for(chrono::milliseconds(15));
                    //   cout << "IN IT " << currentThreads.size() << endl;
                    for (int i = 0; i < currentThreads.size(); ++i) {
                        if (currentThreads.at(i).joinable()) {
                            currentThreads.at(i).join();
                            // delete currentThreads.at(i);
                            currentThreads.erase(
                                std::next(currentThreads.begin(), i));
                        } else {
                            // cout << "Cant join thread " << i << endl;
                        }
                    }
                    // if(currentThreads.size() == 0){ break; }
                }
            }
        }
        cout << " ################# FINISHED FRAME " << endl;
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
