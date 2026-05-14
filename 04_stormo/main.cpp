// Step 04 — N boid, moto rettilineo senza interazioni.

#include "raylib.h"
#include "vec2.h"

// TODO 1: includi <vector>, <cstdlib>, <ctime>


const int LARGHEZZA = 800;
const int ALTEZZA   = 600;

// Numero di boid nello stormo.
// TODO 1 bis: dichiara const int N = 100;


// Raggruppa posizione e velocità di un singolo uccellino.
struct Boid {
    Vec2 pos;
    Vec2 vel;
};

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 04 - Stormo");
    SetTargetFPS(60);

    // Seed del random.
    // srand(time(NULL));

    // TODO 2: crea uno std::vector<Boid> e riempilo con N boid.
    // Per ogni boid:
    //   - pos: x casuale in [0, LARGHEZZA], y casuale in [0, ALTEZZA]
    //   - vel: componenti in [-150, +150]
    // Suggerimento per un float in [a, b]:
    //   ((float)rand()/RAND_MAX) * (b - a) + a


    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // TODO 3: aggiorna posizione di ogni boid con Eulero.
        // for (int i = 0; i < N; ++i) { ... }


        BeginDrawing();
        ClearBackground(BLACK);

        // TODO 4: disegna ogni boid come cerchietto bianco (raggio ~3).


        EndDrawing();
    }

    CloseWindow();
    return 0;
}
