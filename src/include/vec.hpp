#ifndef VEC_H
#define VEC_H

#include <cmath>

template<typename U>
struct VecLength{using type = float; };

template<>
struct VecLength<double> { using type = double; };


template<typename T>
class Vec3 {
public:
    T x{}, y{}, z{};
    Vec3() = default;
    Vec3(T xx, T yy, T zz) :x{xx}, y{yy}, z{zz}{}

    T lengthSquared() const {
        return this->x * this->x + this->y * this->y + this->z * this->z;
    }

    auto length() -> typename VecLength<T>::type const {
        return sqrt(static_cast<typename VecLength<T>::type>(this->lengthSquared()));
    }


    constexpr Vec3<T> operator+(Vec3<T> const& v2) const {
        return Vec3<T>(this->x + v2.x, this->y + v2.y, this->z + v2.z);
    }

    template<typename U>
    auto operator+(Vec3<U> const& v2) const -> Vec3<decltype(T{} + U{})> {
        return { this->x + v2.x, this->y + v2.y, this->z + v2.z};
    }



};
#endif
