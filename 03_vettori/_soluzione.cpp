// Soluzione step 03: file vec2.h completo, allegato come reference.
// (main.cpp resta uguale al template.)
//
// --- vec2.h completo ---
//
// #pragma once
// #include <cmath>
//
// struct Vec2 { float x; float y; };
//
// inline Vec2 add(Vec2 a, Vec2 b)        { return Vec2{ a.x + b.x, a.y + b.y }; }
// inline Vec2 sub(Vec2 a, Vec2 b)        { return Vec2{ a.x - b.x, a.y - b.y }; }
// inline Vec2 mul(Vec2 a, float k)       { return Vec2{ a.x * k, a.y * k }; }
// inline float norm(Vec2 a)              { return sqrtf(a.x * a.x + a.y * a.y); }
// inline Vec2  normalize(Vec2 a) {
//     float n = norm(a);
//     if (n > 0.0f) return Vec2{ a.x / n, a.y / n };
//     return Vec2{ 0, 0 };
// }
// inline float dist(Vec2 a, Vec2 b)      { return norm(sub(a, b)); }

#include "raylib.h"
#include "vec2.h"

const int LARGHEZZA = 800;
const int ALTEZZA   = 600;

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 03 - Vettori");
    SetTargetFPS(60);

    Vec2 pos = { LARGHEZZA / 2.0f, ALTEZZA / 2.0f };
    Vec2 vel = { 150.0f, 0.0f };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        pos = add(pos, mul(vel, dt));
        if (pos.x > LARGHEZZA) pos.x = 0.0f;
        if (pos.x < 0.0f)      pos.x = LARGHEZZA;

        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircle((int)pos.x, (int)pos.y, 8.0f, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
