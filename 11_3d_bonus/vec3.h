// vec3.h — vettore 3D minimale. Estensione naturale di Vec2.
#pragma once
#include <cmath>

struct Vec3 {
    float x, y, z;
};

inline Vec3 add(Vec3 a, Vec3 b)  { return Vec3{ a.x+b.x, a.y+b.y, a.z+b.z }; }
inline Vec3 sub(Vec3 a, Vec3 b)  { return Vec3{ a.x-b.x, a.y-b.y, a.z-b.z }; }
inline Vec3 mul(Vec3 a, float k) { return Vec3{ a.x*k, a.y*k, a.z*k }; }
inline float norm(Vec3 a)        { return sqrtf(a.x*a.x + a.y*a.y + a.z*a.z); }

inline Vec3 normalize(Vec3 a) {
    float n = norm(a);
    if (n > 0.0f) return Vec3{ a.x/n, a.y/n, a.z/n };
    return Vec3{ 0, 0, 0 };
}

inline float dist(Vec3 a, Vec3 b) { return norm(sub(a, b)); }
