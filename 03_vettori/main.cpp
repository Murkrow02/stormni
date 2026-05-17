// Step 03 — Movimento usando struct Vec2.
// Niente di nuovo rispetto allo step 02: serve a testare che vec2.h funzioni.

#include "raylib.h"
#include "vec2.h"

const int LARGHEZZA = 800;
const int ALTEZZA = 600;
const int radius = 20;

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 03 - Vettori");
    SetTargetFPS(60);
    Vec2 pos{LARGHEZZA / 2., ALTEZZA / 2.};
    Vec2 vel{200, 200};

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        pos = add(pos, mul(vel, dt));

        if (pos.x >= LARGHEZZA - radius || pos.x <= 0 + radius) {
            vel.x = -vel.x;
        }
        if (pos.y >= ALTEZZA - radius || pos.y <= 0 + radius) {
            vel.y = -vel.y;
        }


        BeginDrawing();
        ClearBackground(BLACK);

        DrawCircle(pos.x, pos.y, radius, WHITE);
        // DrawCube(Vector3{LARGHEZZA/2., ALTEZZA/2., 50}, 30,30,30, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
