#pragma once
#include <cmath>
#include <iostream>
#include "Vector.hpp"
#include "Buffer.hpp"
using namespace std;
namespace potato {
    template<typename T, typename C>
    struct Line {
        Vec3<T> start {};
        Vec3<T> end {};
        Vec3<C> color {};
    };

    template<typename T>
    struct ImplicitLine {
        Vec3<T> start {};
        Vec3<T> end{};
        T dx {};
        T dy {};
        T c1 {};
        T c2 {};

        ImplicitLine() = default;

        ImplicitLine(Vec3<T> start, Vec3<T> end) {
            this->start = start;
            this->end = end;
            dy = end.y - start.y;
            dx = end.x - start.x;
            c1 = start.x*end.y;
            c2 = end.x*start.y;
        };

        float eval(float x, float y) {
           return -dy*x + dx*y + c1 - c2;
        };

    };

    /*
    template<typename T>
    float implicit( Vec3<T> start, Vec3<T> end, 
                    float x, float y) {
        auto dy = start.y - end.y;
        auto dx = end.x - start.x;
        auto c1 = start.x*end.y;
        auto c2 = end.x*start.y;

        auto res = dy*x + dx*y + c1 - c2;
        return res;
    };

    template<typename T, typename C>
    float implicit(Line<T,C> line, 
                    float x, float y) {
        return implicit(line.start,line.end,x,y);
    };*/



template<typename T, typename C>
class MyLine
{
public:
  Vec3<T> start{ 0, 0, 0 };
  Vec3<T> end{ 1, 1, 1 };

  Vec3<C> color{ 128, 128, 128 };

  MyLine(){};
  MyLine(T x0, T y0, T z0, T x1, T y1, T z1, C r, C g, C b)
    : start(x0, y0, z0)
    , end(x1, y1, z1)
    , color(r, g, b){};

  void paint(Image<Vec3<C>>& image) {
    return this->drawDDA(image);
  };
  //  return drawActualMidpoint(image, this->start, this->end, this->color); };

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
        continue;
      } catch (range_error) {
        continue;
      }
    }
  };

  // inline decltype(T{} / float{}) calculateSlop() const //-> decltype(T{} /
  // float{})
  //  I think this allows doubles, it at least compiles
  //{
  //   return calculateSlop(this->start, this->end);
  // };

  void drawMidpoint(Image<Vec3<C>>& image)
  {
    Vec3<T> myStart(this->start);
    Vec3<T> myEnd(this->end);

    //checkAndFlip(myStart, myEnd);
    //drawActualMidpoint(image, myStart, myEnd, this->color);
  };
};

  /*
template<typename T, typename C>
float
implicit(Line<T, C> line, float x, float y)
{
  return implicit(line.start, line.end, x, y);
};
  */
// void drawLineDDA(Image<Vec3<C>> *image,

}; // namespace potato
