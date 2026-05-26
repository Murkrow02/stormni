#pragma once
#include <cmath>

struct Vec3 {

    float x, y, z;

    inline Vec3 &operator +=(Vec3 const &v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    inline Vec3 &operator *=(float k) {
        x *= k;
        y *= k;
        z *= k;
        return *this;
    }

    inline Vec3 operator-() const {
        return {-x, -y, -z};
    }

    inline Vec3 operator+(Vec3 const &v) const {
        auto result = *this;
        return result += v;
    }

    inline Vec3 operator-(Vec3 const &v) const {
        return {x-v.x, y-v.y, z-v.z};
    }

    inline Vec3 operator*(float k) const {
        auto result = *this;
        return result *= k;
    }

    inline Vec3 operator/(float k) const {
        return *this*(1/k);
    }

    inline bool operator==(Vec3 const & v) const {
        return x==v.x && y==v.y && z==v.z;
    }

    inline bool operator!=(Vec3 const& v) const {
        return!(*this==v);
    }
};

inline static float norm(Vec3 a) { return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z); }

static inline Vec3 normalize(Vec3 a) {
    float n = norm(a);
    if (n > 0.0f) return Vec3{a/n};
    return Vec3{0, 0, 0};
}


inline float dist(Vec3 a, Vec3 b) { return norm(a - b); }