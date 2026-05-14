// Step 06 — Regola 1 di Reynolds: separazione.

#include "raylib.h"
#include "vec2.h"
#include <vector>
#include <cstdlib>
#include <ctime>

const int LARGHEZZA = 800;
const int ALTEZZA   = 600;
const int N         = 100;

// TODO 1: parametri di simulazione.
// const float R_SEP     = 20.0f;
// const float MAX_SPEED = 200.0f;
// const float W_SEP     = 1.5f;

struct Boid { Vec2 pos; Vec2 vel; };

static float casuale(float a, float b) {
    return a + ((float)rand() / RAND_MAX) * (b - a);
}

static Vec2 applica_wrap(Vec2 p) {
    if (p.x < 0)         p.x += LARGHEZZA;
    if (p.x > LARGHEZZA) p.x -= LARGHEZZA;
    if (p.y < 0)         p.y += ALTEZZA;
    if (p.y > ALTEZZA)   p.y -= ALTEZZA;
    return p;
}

// TODO 2: implementa la separazione.
// Per ogni vicino j entro R_SEP, somma a 'spinta' il versore (pos_i - pos_j) / dist.
// Restituisci 'spinta' (NON moltiplicarla qui per il peso o per dt; lo facciamo fuori).
//
// Vec2 calcola_separazione(const std::vector<Boid>& stormo, int i) {
//     Vec2 spinta = {0, 0};
//     for (int j = 0; j < (int)stormo.size(); ++j) {
//         if (j == i) continue;
//         float d = dist(stormo[i].pos, stormo[j].pos);
//         if (d > 0.0f && d < R_SEP) {
//             Vec2 diff = sub(stormo[i].pos, stormo[j].pos);
//             spinta = add(spinta, mul(normalize(diff), 1.0f / d));
//         }
//     }
//     return spinta;
// }

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 06 - Separazione");
    SetTargetFPS(60);
    srand((unsigned)time(NULL));

    std::vector<Boid> stormo;
    for (int i = 0; i < N; ++i) {
        stormo.push_back(Boid{
            { casuale(0, LARGHEZZA), casuale(0, ALTEZZA) },
            { casuale(-150, 150),    casuale(-150, 150)    }
        });
    }

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // TODO 3a: calcola la sterzata di ogni boid in un vettore temporaneo.
        // std::vector<Vec2> sterzate(N);
        // for (int i = 0; i < N; ++i) sterzate[i] = calcola_separazione(stormo, i);

        // TODO 3b: applica sterzata alla velocità, fai clamp, integra posizione, wrap.
        for (int i = 0; i < N; ++i) {
            // stormo[i].vel = add(stormo[i].vel, mul(sterzate[i], W_SEP * dt));
            // float v = norm(stormo[i].vel);
            // if (v > MAX_SPEED) stormo[i].vel = mul(normalize(stormo[i].vel), MAX_SPEED);

            stormo[i].pos = add(stormo[i].pos, mul(stormo[i].vel, dt));
            stormo[i].pos = applica_wrap(stormo[i].pos);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        for (int i = 0; i < N; ++i) {
            DrawCircle((int)stormo[i].pos.x, (int)stormo[i].pos.y, 3.0f, WHITE);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
