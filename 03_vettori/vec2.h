// vec2.h — vettore 2D minimale per il progetto Storni.
// Header-only: tutte le funzioni sono inline qui sotto.

#pragma once
#include <cmath>

struct Vec2 {
    float x;
    float y;
};

// Somma componente per componente.
inline Vec2 add(Vec2 a, Vec2 b) {
    return Vec2{a.x + b.x, a.y + b.y};
}

// TODO 1: sottrazione (a - b).
inline Vec2 sub(Vec2 a, Vec2 b) {
    return Vec2{a.x - b.x, a.y - b.y};
}

// TODO 2: moltiplicazione per scalare (a * k).
inline Vec2 mul(Vec2 a, float k) {
    return Vec2{k * a.x, k * a.y};
}

// TODO 3: norma (lunghezza). Usa sqrtf.
inline float norm(Vec2 a) {
    return sqrtf(a.x * a.x + a.y * a.y);
}

// TODO 4: normalizzazione. Attenzione al vettore nullo!
inline Vec2 normalize(Vec2 a) {
    float n = norm(a);
    if (n > 0.0f) {
        return Vec2{a.x / n, a.y / n};
    }
    return Vec2{0, 0}; // se il vettore è nullo, restituisce (0,0)
}

// TODO 5: distanza fra due punti. Usa sub e norm.
inline float dist(Vec2 a, Vec2 b) {
    return norm(sub(a, b));
}
