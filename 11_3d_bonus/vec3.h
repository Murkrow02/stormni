// vec3.h — vettore 3D minimale. Estensione naturale di Vec2.
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

    inline Vec3 operator-() {
        return {-x, -y, -z};
    }
 // da qui posso anche cacciarli fuori, è meglio?????

    inline Vec3 operator+(Vec3 const &v) {
        auto result = *this;
        return result += v;
    }

    inline Vec3 operator-(Vec3 const &v) {
        auto result = *this;
        return result - v;
    }

    inline Vec3 operator*(float k) {
        auto result = *this;
        return result *= k;
    }

    inline bool operator==(Vec3 const & v) {
        return x==v.x && y==v.y;
    }

    inline bool operator!=(Vec3 const& v) {
        return!(*this==v);
    }


};


inline Vec3 add(Vec3 a, Vec3 b) { return Vec3{a.x + b.x, a.y + b.y, a.z + b.z}; }
inline Vec3 sub(Vec3 a, Vec3 b) { return Vec3{a.x - b.x, a.y - b.y, a.z - b.z}; }
inline Vec3 mul(Vec3 a, float k) { return Vec3{a.x * k, a.y * k, a.z * k}; }
inline float norm(Vec3 a) { return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z); }

inline Vec3 normalize(Vec3 a) {
    float n = norm(a);
    if (n > 0.0f) return Vec3{a.x / n, a.y / n, a.z / n};
    return Vec3{0, 0, 0};
}


// ha senso fare un altro file con le funzioni fisiche?!
inline float dist(Vec3 a, Vec3 b) { return norm(sub(a, b)); }
