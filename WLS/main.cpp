
#include "raylib.h"
#include "vec2.hpp"
#include <vector>
#include <cstdlib>
#include <ctime>

const int LARGHEZZA = 800;
const int ALTEZZA = 600;
const int N = 100;
int radius = 3;

// TODO 1: parametri di simulazione.
const float R_SEP = 50.0f;
const float MAX_SPEED = 200.0f;
const float W_SEP = 250.0f;

struct Boid {
    Vec2 pos;
    Vec2 vel;
};

static float casuale(float a, float b) {
    return a + ((float) rand() / RAND_MAX) * (b - a);
}

static Vec2 applica_wrap(Vec2 p) {
    if (p.x < 0) p.x += LARGHEZZA;
    if (p.x > LARGHEZZA) p.x -= LARGHEZZA;
    if (p.y < 0) p.y += ALTEZZA;
    if (p.y > ALTEZZA) p.y -= ALTEZZA;
    return p;
}

// TODO 2: implementa la separazione.

Vec2 calcola_separazione(const std::vector<Boid> &stormo, int i) {
    Vec2 steer{0, 0};
    for (int j = 0; j != N; ++j) {
        if (i == j) continue;
        float d = norm(sub(stormo[i].pos, stormo[j].pos));
        if (d <= R_SEP) {
            Vec2 v_dis = sub(stormo[i].pos, stormo[j].pos);
            steer = add(steer, mul(v_dis, 1.0f / d));
            continue;
        }
    }
    return steer;
}

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 06 - Separazione");
    SetTargetFPS(60);
    srand((unsigned) time(NULL));

    std::vector<Boid> stormo;
    for (int i = 0; i < N; ++i) {
        stormo.push_back(Boid{
            {casuale(0, LARGHEZZA), casuale(0, ALTEZZA)},
            {casuale(-150, 150), casuale(-150, 150)}
        });
    }

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // TODO 3a: calcola la sterzata di ogni boid in un vettore temporaneo.


        // TODO 3b: applica sterzata alla velocità, fai clamp, integra posizione, wrap.
        for (int i = 0; i < N; ++i) {
            Vec2 steer{0, 0};
            steer = calcola_separazione(stormo, i);
            stormo[i].vel = add(stormo[i].vel, mul(steer, W_SEP * dt));
            stormo[i].vel.x = std::min(stormo[i].vel.x, MAX_SPEED);
            stormo[i].vel.y = std::min(stormo[i].vel.y, MAX_SPEED);
            stormo[i].pos = add(stormo[i].pos, mul(stormo[i].vel, dt));

            // TODO 1: wrap toroidale.
            if (stormo[i].pos.x<=0-radius) {
                stormo[i].pos.x+= LARGHEZZA+radius;
            }

            if (stormo[i].pos.x>=LARGHEZZA+radius) {
                stormo[i].pos.x-= LARGHEZZA-radius;
            }
            if (stormo[i].pos.y<=0-radius) {
                stormo[i].pos.y+= ALTEZZA+radius;
            }

            if (stormo[i].pos.y>=ALTEZZA+radius) {
                stormo[i].pos.y-= ALTEZZA-radius;
            }

        }

        BeginDrawing();
        ClearBackground(BLACK);
        for (int i = 0; i < N; ++i) {
            DrawCircle(stormo[i].pos.x, stormo[i].pos.y, R_SEP, CLITERAL(Color){ 255, 0, 0, 50 });
            DrawCircle(stormo[i].pos.x, stormo[i].pos.y, radius,WHITE);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
