// vec2.h — vettore 2D minimale (versione completa, dallo step 03).
#pragma once
#include <cmath>

struct Vec2 {
    float x;
    float y;
};

inline Vec2 add(Vec2 a, Vec2 b)  { return Vec2{ a.x + b.x, a.y + b.y }; }
inline Vec2 sub(Vec2 a, Vec2 b)  { return Vec2{ a.x - b.x, a.y - b.y }; }
inline Vec2 mul(Vec2 a, float k) { return Vec2{ a.x * k, a.y * k }; }
inline float norm(Vec2 a)        { return sqrtf(a.x * a.x + a.y * a.y); }

inline Vec2 normalize(Vec2 a) {
    float n = norm(a);
    if (n > 0.0f) return Vec2{ a.x / n, a.y / n };
    return Vec2{ 0, 0 };
}

inline float dist(Vec2 a, Vec2 b) { return norm(sub(a, b)); }
