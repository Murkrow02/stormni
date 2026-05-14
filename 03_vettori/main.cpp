// Step 03 — Movimento usando struct Vec2.
// Niente di nuovo rispetto allo step 02: serve a testare che vec2.h funzioni.

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

        // pos = pos + vel * dt, usando le tue funzioni.
        pos = add(pos, mul(vel, dt));

        // Wrap orizzontale.
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
