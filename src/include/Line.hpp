#pragma once
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "Buffer.hpp"
#include "Vector.hpp"
using namespace std;
namespace potato {

template<typename T, typename C>
class Line
{
public:
  Vec3<T> start{ 0, 0, 0 };
  Vec3<T> end{ 1, 1, 1 };

  Vec3<C> color{ 128, 128, 128 };

  Line(){};
  Line(T x0, T y0, T z0, T x1, T y1, T z1, C r, C g, C b) :
    start(x0, y0, z0),
    end(x1, y1, z1),
    color(r, g, b)
  {};

  void paint(Image<Vec3<C>>& image) { return drawDDA(image); };

  void drawDDA(Image<Vec3<C>>& image)
  {

    int dx = this->end.x - this->start.x;
    int dy = this->end.y - this->start.y;

    int steps = abs(dx);
    if (abs(dy) > abs(dx)) {
      steps = abs(dy);
    }
    float xIncrement = float(dx) / float(steps);
    float yIncrement = float(dy) / float(steps);

    float x = this->start.x;
    float y = this->start.y;

    image.setPixel(round(x), round(y), this->color);

    for (int k = 0; k < steps; ++k) {
      try {
        x += xIncrement;
        y += yIncrement;

        image.setPixel(round(x), round(y), this->color);
      } catch (out_of_range) {
        break;
      }
    }
  };
};

}; // namespace potato
