// Step 07 — Regola 2 di Reynolds: allineamento.

#include "raylib.h"
#include "vec2.h"
#include <vector>
#include <cstdlib>
#include <ctime>

const int LARGHEZZA = 800;
const int ALTEZZA = 600;
const int N = 100;
const int R_BOID = 5;

const float R_SEP = 20.0f;
const float MAX_SPEED = 200.0f;
const float W_SEP = 2.0f;
const float R_VIEW = 50.0f;
const float W_ALL = 1.0f;

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

static Vec2 calcola_separazione(const std::vector<Boid> &stormo, int i) {
    Vec2 spinta = {0, 0};
    for (int j = 0; j < (int) stormo.size(); ++j) {
        if (j == i) continue;
        float d = dist(stormo[i].pos, stormo[j].pos);
        if (d > 0.0f && d < R_SEP) {
            Vec2 diff = sub(stormo[i].pos, stormo[j].pos);
            spinta = add(spinta, mul(normalize(diff), 1.0f / d));
        }
    }
    return spinta;
}

Vec2 calcola_allineamento(const std::vector<Boid> &stormo, int i) {
    Vec2 v_sum{0, 0};
    float tot = 0;
    for (int j = 0; j != N; ++j) {
        if (j == i) continue;
        float d = dist(stormo[i].pos, stormo[j].pos);
        if (d <= R_VIEW) {
            v_sum = add(mul(normalize(v_sum), MAX_SPEED), stormo[j].vel);
            tot += 1;
        }
    }
    if (tot == 0) { return v_sum; }
    Vec2 v_cm = mul(v_sum, 1 / tot);
    Vec2 steer_all = sub(v_cm, stormo[i].vel);
    return steer_all;
}

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 07 - Allineamento");
    SetTargetFPS(60);
    srand((unsigned) time(NULL));

    std::vector<Boid> stormo;
    for (int i = 0; i < N; ++i) {
        stormo.push_back(Boid{
            {casuale(0, LARGHEZZA), casuale(0, ALTEZZA)},
            {casuale(-350, 350), casuale(-350, 350)}
        });
    }

    std::vector<Vec2> sep(N), all_(N);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Fase 1: calcola tutte le sterzate.
        for (int i = 0; i < N; ++i) {
            Vec2 steer_sep{0, 0};
            Vec2 steer_all{0, 0};
            steer_sep = calcola_separazione(stormo, i);
            steer_all = calcola_allineamento(stormo, i);
            stormo[i].vel = add(stormo[i].vel, add(mul(steer_sep,W_SEP*dt),mul(steer_all, W_ALL*dt)));
            stormo[i].vel.x = std::min(stormo[i].vel.x, MAX_SPEED);
            stormo[i].vel.y = std::min(stormo[i].vel.y, MAX_SPEED);
            stormo[i].pos = add(stormo[i].pos, mul(stormo[i].vel, dt));
            stormo[i].pos = applica_wrap(stormo[i].pos);
        }


        BeginDrawing();
        ClearBackground(BLACK);
        for (int i = 0; i < N; ++i) {
            DrawCircle(stormo[i].pos.x, stormo[i].pos.y, R_SEP, CLITERAL(Color){ 255, 0, 0, 50 });
            DrawCircle(stormo[i].pos.x, stormo[i].pos.y, R_VIEW, CLITERAL(Color){ 0, 255, 0, 50 });
            DrawCircle(stormo[i].pos.x, stormo[i].pos.y, R_BOID, WHITE);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
