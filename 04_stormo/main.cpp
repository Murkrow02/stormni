// Step 04 — N boid, moto rettilineo senza interazioni.

#include <vector>
#include <random>
#include "raylib.h"
#include "vec2.h"
#include "boid.h"

// TODO 1: includi <vector>, <cstdlib>, <ctime>

const int LARGHEZZA = 800;
const int ALTEZZA = 600;
const int N = 100;
const int radius = 3;

// Numero di boid nello stormo.
// TODO 1 bis: dichiara const int N = 100;


int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 04 - Stormo");
    SetTargetFPS(60);
    std::vector<Boid> stormo;
    std::default_random_engine gen;
    std::uniform_real_distribution<float> dist_posx(0, LARGHEZZA);
    std::uniform_real_distribution<float> dist_posy(0, ALTEZZA);
    std::uniform_real_distribution<float> dist_vel(-150, 150);
    for (int i = 0; i != N; ++i) {
        Vec2 pos{dist_posx(gen), dist_posy(gen)};
        Vec2 vel{dist_vel(gen), dist_vel(gen)};
        Boid boid{pos, vel};
        stormo.push_back(boid);
    }


    // TODO 2: crea uno std::vector<Boid> e riempilo con N boid.


    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // TODO 3: aggiorna posizione di ogni boid con Eulero.
        for (int i = 0; i != N; ++i) {
            stormo[i].pos = add(stormo[i].pos, mul(stormo[i].vel, dt));
            if (stormo[i].pos.x >= LARGHEZZA - radius || stormo[i].pos.x <= 0 + radius) {
                stormo[i].vel.x = -stormo[i].vel.x;
            }
            if (stormo[i].pos.y >= ALTEZZA - radius || stormo[i].pos.y <= 0 + radius) {
                stormo[i].vel.y = -stormo[i].vel.y;
            }
        }


        BeginDrawing();
        ClearBackground(BLACK);

        // TODO 4: disegna ogni boid come cerchietto bianco (raggio ~3).
        for (int i = 0; i != N; ++i) {
            DrawCircle(stormo[i].pos.x, stormo[i].pos.y, radius, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
