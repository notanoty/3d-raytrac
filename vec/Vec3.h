//
// Created by Timofei Arefev on 09/06/2025.
//

// Vec3.h
#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>

template<typename T>
class Vec3 {
public:
    T x, y, z;

    Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
    Vec3(T v) : x(v), y(v), z(v) {}
    Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

    Vec3<T>& normalize() {
        T len2 = length2();
        if (len2 > 0) {
            T invLen = 1 / std::sqrt(len2);
            x *= invLen; y *= invLen; z *= invLen;
        }
        return *this;
    }

    T length2() const { return x * x + y * y + z * z; }
    T length() const { return std::sqrt(length2()); }

    T dot(const Vec3<T>& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vec3<T> operator * (const T &f) const { return Vec3<T>(x * f, y * f, z * f); }
    Vec3<T> operator * (const Vec3<T> &v) const { return Vec3<T>(x * v.x, y * v.y, z * v.z); }
    Vec3<T> operator + (const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
    Vec3<T> operator - (const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
    Vec3<T>& operator += (const Vec3<T> &v) { x += v.x; y += v.y; z += v.z; return *this; }
    Vec3<T>& operator *= (const Vec3<T> &v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
    Vec3<T> operator - () const { return Vec3<T>(-x, -y, -z); }

    friend std::ostream& operator << (std::ostream& os, const Vec3<T>& v) {
        os << "[" << v.x << " " << v.y << " " << v.z << "]";
        return os;
    }
};

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

#endif // VEC3_H
