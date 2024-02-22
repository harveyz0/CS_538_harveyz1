#pragma once

#include <cmath>
#include <iostream>
using namespace std;

namespace potato {
template<typename T>
struct VecLength
{
  using type = float;
};

template<>
struct VecLength<double>
{
  using type = double;
};

template<typename T>
struct Vec4;

template<typename T>
class Vec3
{
public:
  union
  {
    T x{}, r, s;
  };
  union
  {
    T y{}, g, t;
  };
  union
  {
    T z{}, b, p;
  };

  Vec3() = default;
  // Vec3(Vec3<T> &v) = default;
  Vec3(T x, T y, T z)
    : x(x)
    , y(y)
    , z(z){};

  ////////////////////////////////////////////////////////
  // VEC4
  ////////////////////////////////////////////////////////

  ////////////////////////////////////////////////////////
  // VEC3
  ////////////////////////////////////////////////////////

  template<typename U>
  Vec3(const Vec3<U>& other)
    : Vec3<T>(static_cast<T>(other.x),
              static_cast<T>(other.y),
              static_cast<T>(other.z)){};

  void copy(const Vec3<T>& from)
  {
    this->x = from.x;
    this->y = from.y;
    this->z = from.z;
  };
  Vec3(Vec4<T>& other)
  {
    x = other.x;
    y = other.y;
    z = other.z;
  };

  auto length() const -> typename VecLength<T>::type
  {
    return sqrt(
      static_cast<typename VecLength<T>::type>(x * x + y * y + z * z));
  };

  template<typename U>
  auto operator+(const Vec3<U>& v) const -> Vec3<decltype(T{} + U{})>
  {
    return { x + v.x, y + v.y, z + v.z };
  };

  template<typename U>
  auto operator-(const Vec3<U>& v) const -> Vec3<decltype(T{} - U{})>
  {
    return { x - v.x, y - v.y, z - v.z };
  };

  template<typename U>
  auto operator*(const Vec3<U>& rightOp) const -> Vec3<decltype(T{} * U{})>
  {
    return { this->x * rightOp.x, this->y * rightOp.y, this->z * rightOp.z };
  }

  template<typename U>
  auto operator*(const U& rightOp) const -> Vec3<decltype(T{} * U{})>
  {
    return { this->x * rightOp, this->y * rightOp, this->z * rightOp };
  }
  // This didn't work, because Vec3<T> also matchs the type of U
  //        template<typename U>
  //        friend auto operator*(const Vec3<T> &leftOp, const Vec3<U>
  //        &rightOp)
  //        -> Vec3<decltype(T{} * U{})>{
  //            return { leftOp.x * rightOp.x, leftOp.y * rightOp.y, leftOp.z *
  //            rightOp.z };
  //        }
  //
  //        template<typename U>
  //        friend auto operator*(const Vec3<T> &leftOp, const U &rightOp) ->
  //        Vec3<decltype(T{} * U{})>{
  //            return { leftOp.x * rightOp, leftOp.y * rightOp, leftOp.z *
  //            rightOp };
  //        }
  //
  //        template<typename U>
  //        friend auto operator*(const U &leftOp, const Vec3<T> &rightOp) ->
  //        Vec3<decltype(T{} * U{})>{
  //            return rightOp * leftOp;
  //        }

  template<typename U>
  friend auto operator/(const Vec3<T>& leftOp, const U& rightOp)
    -> Vec3<decltype(T{} / U{})>
  {
    return { leftOp.x / rightOp, leftOp.y / rightOp, leftOp.z / rightOp };
  }

  template<typename U>
  friend auto operator/(const U& leftOp, const Vec3<T>& rightOp)
    -> Vec3<decltype(T{} / U{})>
  {
    return rightOp / leftOp;
  }

  friend ostream& operator<<(ostream& os, Vec3<T> v)
  {
    return os << "(" << v.x << "," << v.y << "," << v.z << ")";
  };

  T operator[](int i) const
  {
    switch (i) {
      case 0:
        return x;
      case 1:
        return y;
      case 2:
        return z;
      default:
        throw std::out_of_range("ERROR: " + to_string(i) + " out of bounds!");
    }
  };

  T& operator[](int i)
  {
    switch (i) {
      case 0:
        return x;
      case 1:
        return y;
      case 2:
        return z;
      default:
        throw std::out_of_range("ERROR: " + to_string(i) + " out of bounds!");
    }
  };

  template<typename U>
  auto dot(const Vec3<U>& rightOp) const -> decltype(T{} * U{})
  {
    return this->x * rightOp.x + this->y * rightOp.y + this->z * rightOp.z;
  };

  auto normalize() const -> Vec3<typename VecLength<T>::type>
  {
    return { static_cast<typename VecLength<T>::type>(this->x / this->length()),
             static_cast<typename VecLength<T>::type>(this->y / this->length()),
             static_cast<typename VecLength<T>::type>(this->z /
                                                      this->length()) };
  };

  template<typename U>
  auto cross(const Vec3<U>& rightOp) const -> Vec3<decltype(T{} * U{})>
  {
    return { this->y * rightOp.z - this->z * rightOp.y,
             this->z * rightOp.x - this->x * rightOp.z,
             this->x * rightOp.y - this->y * rightOp.x };
  };
};

using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;
using Vec3d = Vec3<double>;
using Vec3u = Vec3<unsigned char>;
template<typename T>
class Vec4
{
  union
  {
    T x{}, r, s;
  };
  union
  {
    T y{}, g, t;
  };
  union
  {
    T z{}, b, p;
  };
  union
  {
    T w{}, a, q;
  };

  Vec4() = default;
  Vec4(T x, T y, T z, T w)
    : x(x)
    , y(y)
    , z(z)
    , w(w){};

  template<typename U>
  Vec4(const Vec4<U>& other)
  {
    x = static_cast<T>(other.x);
    y = static_cast<T>(other.y);
    z = static_cast<T>(other.z);
    w = static_cast<T>(other.w);
  };

  Vec4(Vec3<T>& other, T w)
  {
    x = other.x;
    y = other.y;
    z = other.z;
    this->w = w;
  };

  auto length() -> typename VecLength<T>::type
  {
    return sqrt(
      static_cast<typename VecLength<T>::type>(x * x + y * y + z * z));
  };

  template<typename U>
  auto operator+(Vec4<U> v) const -> Vec4<decltype(T{} + U{})>
  {
    return { x + v.x, y + v.y, z + v.z };
  };

  template<typename U>
  auto operator-(Vec4<U> v) const -> Vec4<decltype(T{} - U{})>
  {
    return { x - v.x, y - v.y, z - v.z };
  };

  friend ostream& operator<<(ostream& os, Vec4<T> v)
  {
    os << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")";
    return os;
  };

  T& operator[](int i)
  {
    switch (i) {
      case 0:
        return x;
      case 1:
        return y;
      case 2:
        return z;
      case 3:
        return w;
      default:
        throw std::out_of_range("ERROR: " + to_string(i) + " out of bounds!");
    }
  };
};

using Vec4f = Vec4<float>;
using Vec4i = Vec4<int>;
using Vec4d = Vec4<double>;
using Vec4u = Vec4<unsigned char>;

}; // namespace potato
