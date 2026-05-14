// Soluzione step 02.
#include "raylib.h"

const int LARGHEZZA = 800;
const int ALTEZZA   = 600;

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 02 - Movimento");
    SetTargetFPS(60);

    // Stato: posizione e velocità del punto.
    float pos_x = LARGHEZZA / 2.0f;
    float pos_y = ALTEZZA   / 2.0f;
    float vel_x = 150.0f;  // pixel/secondo verso destra
    float vel_y = 0.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Integrazione di Eulero esplicito.
        pos_x += vel_x * dt;
        pos_y += vel_y * dt;

        // Wrap orizzontale.
        if (pos_x > LARGHEZZA) pos_x = 0.0f;
        if (pos_x < 0.0f)      pos_x = LARGHEZZA;

        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircle((int)pos_x, (int)pos_y, 8.0f, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
