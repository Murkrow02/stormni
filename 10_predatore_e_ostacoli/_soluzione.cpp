// Soluzione step 10 — predatore + ostacoli.
#include "raylib.h"
#include "vec2.h"
#include <vector>
#include <cstdlib>
#include <ctime>

const int LARGHEZZA = 800;
const int ALTEZZA   = 600;
const int N         = 100;

const float R_SEP     = 20.0f;
const float R_VIEW    = 50.0f;
const float MAX_SPEED = 200.0f;
const float W_SEP     = 1.5f;
const float W_ALL     = 1.0f;
const float W_COES    = 1.0f;
const float R_FUGA    = 80.0f;
const float W_FUGA    = 5.0f;
const float W_OST     = 2.0f;

struct Boid     { Vec2 pos; Vec2 vel; };
struct Ostacolo { Vec2 pos; float raggio; };

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
static Vec2 calcola_separazione(const std::vector<Boid>& s, int i) {
    Vec2 spinta = {0, 0};
    for (int j = 0; j < (int)s.size(); ++j) {
        if (j == i) continue;
        float d = dist(s[i].pos, s[j].pos);
        if (d > 0.0f && d < R_SEP)
            spinta = add(spinta, mul(normalize(sub(s[i].pos, s[j].pos)), 1.0f / d));
    }
    return spinta;
}
static Vec2 calcola_allineamento(const std::vector<Boid>& s, int i) {
    Vec2 somma = {0, 0}; int k = 0;
    for (int j = 0; j < (int)s.size(); ++j) {
        if (j == i) continue;
        float d = dist(s[i].pos, s[j].pos);
        if (d > 0.0f && d < R_VIEW) { somma = add(somma, s[j].vel); k++; }
    }
    if (k == 0) return Vec2{0, 0};
    return sub(mul(somma, 1.0f / k), s[i].vel);
}
static Vec2 calcola_coesione(const std::vector<Boid>& s, int i) {
    Vec2 somma = {0, 0}; int k = 0;
    for (int j = 0; j < (int)s.size(); ++j) {
        if (j == i) continue;
        float d = dist(s[i].pos, s[j].pos);
        if (d > 0.0f && d < R_VIEW) { somma = add(somma, s[j].pos); k++; }
    }
    if (k == 0) return Vec2{0, 0};
    return sub(mul(somma, 1.0f / k), s[i].pos);
}
static Vec2 calcola_fuga(const Boid& b, Vec2 predatore) {
    float d = dist(b.pos, predatore);
    if (d > 0.0f && d < R_FUGA)
        return mul(normalize(sub(b.pos, predatore)), 1.0f);
    return Vec2{0, 0};
}
static Vec2 calcola_ostacoli(const Boid& b, const std::vector<Ostacolo>& ost) {
    Vec2 spinta = {0, 0};
    for (const auto& o : ost) {
        float d = dist(b.pos, o.pos);
        float soglia = o.raggio + 30.0f;
        if (d > 0.0f && d < soglia)
            spinta = add(spinta, mul(normalize(sub(b.pos, o.pos)), (soglia - d) / soglia));
    }
    return spinta;
}

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 10 - Predatore e Ostacoli");
    SetTargetFPS(60);
    srand((unsigned)time(NULL));

    std::vector<Boid> stormo;
    for (int i = 0; i < N; ++i)
        stormo.push_back(Boid{
            { casuale(0, LARGHEZZA), casuale(0, ALTEZZA) },
            { casuale(-150, 150),    casuale(-150, 150)    }
        });

    std::vector<Ostacolo> ostacoli = {
        { {200, 300}, 40 },
        { {500, 200}, 60 },
        { {600, 450}, 30 }
    };

    std::vector<Vec2> sep(N), all_(N), coes(N);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        Vec2 predatore = { (float)GetMouseX(), (float)GetMouseY() };

        for (int i = 0; i < N; ++i) {
            sep[i]  = calcola_separazione(stormo, i);
            all_[i] = calcola_allineamento(stormo, i);
            coes[i] = calcola_coesione(stormo, i);
        }
        for (int i = 0; i < N; ++i) {
            Vec2 acc = add(add(mul(sep[i], W_SEP), mul(all_[i], W_ALL)), mul(coes[i], W_COES));
            acc = add(acc, mul(calcola_fuga(stormo[i], predatore),     W_FUGA));
            acc = add(acc, mul(calcola_ostacoli(stormo[i], ostacoli),  W_OST));

            stormo[i].vel = add(stormo[i].vel, mul(acc, 60.0f * dt));
            float v = norm(stormo[i].vel);
            if (v > MAX_SPEED) stormo[i].vel = mul(normalize(stormo[i].vel), MAX_SPEED);
            stormo[i].pos = add(stormo[i].pos, mul(stormo[i].vel, dt));
            stormo[i].pos = applica_wrap(stormo[i].pos);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        for (const auto& o : ostacoli)
            DrawCircle((int)o.pos.x, (int)o.pos.y, o.raggio, GRAY);
        for (int i = 0; i < N; ++i)
            DrawCircle((int)stormo[i].pos.x, (int)stormo[i].pos.y, 3.0f, WHITE);
        DrawCircleLines((int)predatore.x, (int)predatore.y, R_FUGA, MAROON);
        DrawCircle     ((int)predatore.x, (int)predatore.y, 8.0f,    RED);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
