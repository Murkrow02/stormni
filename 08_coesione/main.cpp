// Step 08 — Regola 3 di Reynolds: coesione. Tutte e 3 insieme = murmuration.

#include "raylib.h"
#include "vec2.h"
#include <vector>
#include <cstdlib>
#include <ctime>

const int LARGHEZZA = 1800;
const int ALTEZZA = 900;
const int N = 100;
const int R_BOID = 5;
const float R_SEP = 20.0f;
const float R_VIEW = 50.0f;
const float MAX_SPEED = 200.0f;
const float W_SEP = 3.5f;
const float W_ALL = 1.0f;
const float W_COES = 3.0f;

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

static Vec2 calcola_separazione(const std::vector<Boid> &s, int i) {
    Vec2 spinta = {0, 0};
    for (int j = 0; j < (int) s.size(); ++j) {
        if (j == i) continue;
        float d = dist(s[i].pos, s[j].pos);
        if (d > 0.0f && d < R_SEP)
            spinta = add(spinta, mul(normalize(sub(s[i].pos, s[j].pos)), 1.0f / d));
    }
    return spinta;
}

static Vec2 calcola_allineamento(const std::vector<Boid> &s, int i) {
    Vec2 somma = {0, 0};
    int k = 0;
    for (int j = 0; j < (int) s.size(); ++j) {
        if (j == i) continue;
        float d = dist(s[i].pos, s[j].pos);
        if (d > 0.0f && d < R_VIEW) {
            somma = add(somma, s[j].vel);
            k++;
        }
    }
    if (k == 0) return Vec2{0, 0};
    return sub(mul(somma, 1.0f / k), s[i].vel);
}

// TODO 2: scrivi calcola_coesione.
Vec2 calcola_coesione(const std::vector<Boid> &s, int i) {
    Vec2 somma = {0, 0};
    int k = 0;
    for (int j = 0; j < (int) s.size(); ++j) {
        if (j == i) continue;
        float d = dist(s[i].pos, s[j].pos);
        if (d > 0.0f && d < R_VIEW) {
            somma = add(somma, s[j].pos);
            k++;
        }
    }
    if (k == 0) return Vec2{0, 0};
    return sub(mul(somma, 1.0f / k), s[i].pos);
}

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 08 - Coesione (Murmuration!)");
    SetTargetFPS(60);
    srand((unsigned) time(NULL));

    std::vector<Boid> stormo;
    for (int i = 0; i < N; ++i)
        stormo.push_back(Boid{
            {casuale(0, LARGHEZZA), casuale(0, ALTEZZA)},
            {casuale(-150, 150), casuale(-150, 150)}
        });

    std::vector<Vec2> sep(N), all_(N), coes(N);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        for (int i = 0; i < N; ++i) {
            sep[i] = calcola_separazione(stormo, i);
            all_[i] = calcola_allineamento(stormo, i);
            coes[i] = calcola_coesione(stormo, i);

            Vec2 acc = add(add(mul(sep[i], W_SEP), mul(all_[i], W_ALL)), mul(coes[i], W_COES));
            stormo[i].vel = add(stormo[i].vel, mul(acc, dt));
            stormo[i].vel.x = std::min(stormo[i].vel.x, MAX_SPEED);
            stormo[i].vel.y = std::min(stormo[i].vel.y, MAX_SPEED);
            stormo[i].pos = add(stormo[i].pos, mul(stormo[i].vel, dt));
            stormo[i].pos = applica_wrap(stormo[i].pos);
        }

        BeginDrawing();
        for (int i = 0; i != N; ++i) {
            DrawCircle(stormo[i].pos.x, stormo[i].pos.y, R_SEP, CLITERAL(Color){255, 0, 0, 100});
            DrawCircle(stormo[i].pos.x, stormo[i].pos.y, R_VIEW, CLITERAL(Color){0, 255, 0, 50});
            DrawCircle(stormo[i].pos.x, stormo[i].pos.y, R_BOID, WHITE);
            ClearBackground(BLACK);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
