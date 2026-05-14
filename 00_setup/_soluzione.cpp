// Soluzione step 00: identica a main.cpp (qui non c'è nulla da fare).
#include "raylib.h"

const int LARGHEZZA = 800;
const int ALTEZZA   = 600;

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 00 - Setup");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
