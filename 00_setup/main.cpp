// Step 00 — Setup.
// Verifica che raylib compili e si apra una finestra.
// Qui non c'è nulla da modificare: serve solo per testare la toolchain.

#include "raylib.h"

// Dimensioni della finestra in pixel.
const int LARGHEZZA = 800;
const int ALTEZZA   = 600;

// ciao

int main() {
    // Crea la finestra (titolo, dimensioni).
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 00 - Setup");

    // Limita il loop a 60 frame al secondo.
    SetTargetFPS(60);

    // Loop principale: gira finché l'utente non preme ESC o chiude la finestra.
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        // Niente da disegnare ancora: lo sfondo nero basta.
        EndDrawing();
    }

    // Chiude la finestra e libera la memoria di raylib.
    CloseWindow();
    return 0;
}
