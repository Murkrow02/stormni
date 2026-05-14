// Step 07 — Regola 2 di Reynolds: allineamento.

#include "raylib.h"
#include "vec2.h"
#include <vector>
#include <cstdlib>
#include <ctime>

const int LARGHEZZA = 800;
const int ALTEZZA   = 600;
const int N         = 100;

const float R_SEP     = 20.0f;
const float MAX_SPEED = 200.0f;
const float W_SEP     = 1.5f;

// TODO 1: aggiungi R_VIEW e W_ALL.
// const float R_VIEW = 50.0f;
// const float W_ALL  = 1.0f;

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

static Vec2 calcola_separazione(const std::vector<Boid>& stormo, int i) {
    Vec2 spinta = {0, 0};
    for (int j = 0; j < (int)stormo.size(); ++j) {
        if (j == i) continue;
        float d = dist(stormo[i].pos, stormo[j].pos);
        if (d > 0.0f && d < R_SEP) {
            Vec2 diff = sub(stormo[i].pos, stormo[j].pos);
            spinta = add(spinta, mul(normalize(diff), 1.0f / d));
        }
    }
    return spinta;
}

// TODO 2: scrivi calcola_allineamento.
// Vec2 calcola_allineamento(const std::vector<Boid>& stormo, int i) {
//     Vec2 somma_vel = {0, 0};
//     int k = 0;
//     for (int j = 0; j < (int)stormo.size(); ++j) {
//         if (j == i) continue;
//         float d = dist(stormo[i].pos, stormo[j].pos);
//         if (d > 0.0f && d < R_VIEW) {
//             somma_vel = add(somma_vel, stormo[j].vel);
//             k++;
//         }
//     }
//     if (k == 0) return Vec2{0, 0};
//     Vec2 v_media = mul(somma_vel, 1.0f / k);
//     return sub(v_media, stormo[i].vel);
// }

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 07 - Allineamento");
    SetTargetFPS(60);
    srand((unsigned)time(NULL));

    std::vector<Boid> stormo;
    for (int i = 0; i < N; ++i) {
        stormo.push_back(Boid{
            { casuale(0, LARGHEZZA), casuale(0, ALTEZZA) },
            { casuale(-150, 150),    casuale(-150, 150)    }
        });
    }

    std::vector<Vec2> sep(N), all_(N);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Fase 1: calcola tutte le sterzate.
        for (int i = 0; i < N; ++i) {
            sep[i]  = calcola_separazione(stormo, i);
            // TODO 3a: all_[i] = calcola_allineamento(stormo, i);
        }

        // Fase 2: applica.
        for (int i = 0; i < N; ++i) {
            Vec2 acc = mul(sep[i], W_SEP);
            // TODO 3b: somma il contributo di allineamento.
            // acc = add(acc, mul(all_[i], W_ALL));

            stormo[i].vel = add(stormo[i].vel, mul(acc, 60.0f * dt));

            float v = norm(stormo[i].vel);
            if (v > MAX_SPEED) stormo[i].vel = mul(normalize(stormo[i].vel), MAX_SPEED);

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
