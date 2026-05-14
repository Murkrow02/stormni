// Soluzione step 01.
#include "raylib.h"

const int LARGHEZZA = 800;
const int ALTEZZA   = 600;

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 01 - Punto");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Cerchio bianco al centro, raggio 8 pixel.
        DrawCircle(LARGHEZZA / 2, ALTEZZA / 2, 8.0f, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
