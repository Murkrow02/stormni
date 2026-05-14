// Step 01 — Disegna un punto fermo al centro.

#include "raylib.h"

const int LARGHEZZA = 800;
const int ALTEZZA   = 600;

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 01 - Punto");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // TODO 1: disegna un cerchio bianco al centro della finestra.
        // Suggerimento: DrawCircle(x, y, raggio, colore).
        // Le coordinate del centro sono (LARGHEZZA/2, ALTEZZA/2).


        EndDrawing();
    }

    CloseWindow();
    return 0;
}
