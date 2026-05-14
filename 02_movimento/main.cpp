// Step 02 — Un punto che si muove con velocità costante.

#include "raylib.h"

const int LARGHEZZA = 800;
const int ALTEZZA   = 600;

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 02 - Movimento");
    SetTargetFPS(60);

    // TODO 1: dichiara pos_x, pos_y, vel_x, vel_y (tipo float).
    // Posizione iniziale: centro finestra.
    // Velocità iniziale: orizzontale verso destra, es. 150 pixel/secondo.


    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // TODO 2: aggiorna posizione con Eulero: pos += vel * dt.


        // TODO 3: wrap semplice. Se pos_x supera LARGHEZZA, riportala a 0.


        BeginDrawing();
        ClearBackground(BLACK);

        // TODO 4: disegna il cerchio in (pos_x, pos_y).


        EndDrawing();
    }

    CloseWindow();
    return 0;
}
