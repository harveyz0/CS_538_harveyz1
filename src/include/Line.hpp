#pragma once
#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "Buffer.hpp"
#include "Vector.hpp"
using namespace std;
namespace potato {

    template<typename T>
    struct ImplicitLine {
        Vec3<T> start {};
        Vec3<T> end{};
        T dx {};
        T dy {};
        T c1 {};
        T c2 {};

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

template<typename T>
bool
checkAndFlip(Vec3<T>& start, Vec3<T>& end)
{
  decltype(T{} / float{}) slope = calculateSlop(start, end);


  if (slope < 0) {
    Vec3<T> temp(start);
    start.copy(end);
    end.copy(temp);
    return true;
  }

  return false;
  };
    /*
    template<typename T>
    float implicit( Vec3<T> start, Vec3<T> end,
                    float x, float y) {
        auto dy = start.y - end.y;
        auto dx = end.x - start.x;
        auto c1 = start.x*end.y;
        auto c2 = end.x*start.y;


    template<typename T, typename C>
    float implicit(Line<T,C> line,
                    float x, float y) {
        return implicit(line.start,line.end,x,y);
    };

  // T dx = end.x - start.x;
  // T dy = end.y - start.y;

  //  if (abs(dx) < abs(dy)) {
  //    swap(start.x, start.y);
  //    swap(end.x, end.y);
  //  }
template<typename T>
float
implicit(Vec3<T> start, Vec3<T> end, float x, float y)
{
  auto dy = start.y - end.y;
  auto dx = end.x - start.x;
  auto c1 = start.x * end.y;
  auto c2 = end.x * start.y;

  auto res = dy * x + dx * y + c1 - c2;
  return res;
};

  */
template<typename T>
inline float
calculateMidpoint(float x, float y, Vec3<T> start, Vec3<T> end)
// This should probably be fixed to allow doubles
{
  return (start.y - end.y) * x + (end.x - start.x) * y + (start.x * end.y) -
         (end.x * start.y);
};

template<typename T>
float
calculateMidpoint(Vec3<T> start, Vec3<T> end, float x, float y)
{
  auto dy = start.y - end.y;
  auto dx = end.x - start.x;
  auto c1 = start.x * end.y;
  auto c2 = end.x * start.y;

  auto res = dy * x + dx * y + c1 - c2;
  return res;
};

template<typename T, typename C>
void
drawActualMidpoint(Image<Vec3<C>>& image,
                   const Vec3<T>& start,
                   const Vec3<T>& end,
                   const Vec3<C>& color)
{
  // When you need to flip x and y do all xs and ys flip?
  Vec3<T> newStart(start);
  Vec3<T> newEnd(end);
  // if(calculateSlop(start, end) < 0){
  //   newStart.copy(end);
  //   newEnd.copy(start);
  // }
  // checkAndFlip(newStart, newEnd);

  int dx = newEnd.x - newStart.x;
  int dy = newEnd.y - newStart.y;
  bool swap = abs(dx) < abs(dy);
  if (swap) {
    std::swap(dx, dy);
    std::swap(newStart.x, newStart.y);
    std::swap(newEnd.x, newEnd.y);
  }
  if (dx < 0) {
    std::swap(newStart, newEnd);
    // auto temp = newEnd;
    // newEnd = newStart;
    // newStart = temp;
    dx = -dx;
    dy = -dy;
  }

  decltype(T{} / float{}) y = newStart.y;
  // decltype(T{} / float{}) changeToY = 1;
  decltype(T{} / float{}) change = 1;
  decltype(T{} / float{}) x = newStart.x;
  decltype(T{} / float{}) stop = newEnd.x;
  if (dy < 0) {
    change = -1;
    dy = -dy;
  }

  float d = calculateMidpoint(x + 1, y + 0.5, newStart, newEnd);
  for (; x <= stop; ++x) {
    if (swap) {
      image.setPixel(y, x, color);
    } else {
      image.setPixel(x, y, color);
    }
    if (d < 0) {
      y = y + change;
      d = d + dx - dy;
    } else {
      d = d - dy;
    }
  }
};

template<typename T>
inline auto
calculateSlop(const Vec3<T> start, const Vec3<T> end) -> decltype(T{} / float{})
// I think this allows doubles, it at least compiles
{
  return decltype(T{} / float{})(end.y - start.y) /
         decltype(T{} / float{})(end.x - start.x);
};

template<typename T, typename C>
class Line
{
public:
  Vec3<T> start{ 0, 0, 0 };
  Vec3<T> end{ 1, 1, 1 };

  Vec3<C> color{ 128, 128, 128 };

  Line(){};
  Line(T x0, T y0, T z0, T x1, T y1, T z1, C r, C g, C b)
    : start(x0, y0, z0)
    , end(x1, y1, z1)
    , color(r, g, b){};

  void paint(Image<Vec3<C>>& image) { return drawMidpoint(image); };

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

    checkAndFlip(myStart, myEnd);
    drawActualMidpoint(image, myStart, myEnd, this->color);
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
template<typename T, typename C>
void
RealedrawLineDDA(Image<Vec3<C>>* image, Line<T, C>& line)
{
  auto dx = line.end.x - line.start.x;
  auto dy = line.end.y - line.start.y;
  int steps = 0;
  float xInc, yInc;
  float x = line.start.x;
  float y = line.start.y;

  if (abs(dx) > abs(dy)) {
    steps = abs(dx);
  } else {
    steps = abs(dy);
  }

  xInc = float(dx) / float(steps);
  yInc = float(dy) / float(steps);

  image->setPixel((int)round(x), (int)round(y), line.color);

  for (int k = 0; k < steps; k++) {
    x += xInc;
    y += yInc;

    image->setPixel((int)round(x), (int)round(y), line.color);
  }
};

}; // namespace potato
