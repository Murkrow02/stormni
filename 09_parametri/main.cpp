// Step 09 — Tara i pesi delle 3 regole con la tastiera, in tempo reale.

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

// TODO 1: rendi questi tre NON const (modificabili a runtime).
float W_SEP  = 1.5f;
float W_ALL  = 1.0f;
float W_COES = 1.0f;

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
    Vec2 somma = {0, 0};
    int k = 0;
    for (int j = 0; j < (int)s.size(); ++j) {
        if (j == i) continue;
        float d = dist(s[i].pos, s[j].pos);
        if (d > 0.0f && d < R_VIEW) { somma = add(somma, s[j].vel); k++; }
    }
    if (k == 0) return Vec2{0, 0};
    return sub(mul(somma, 1.0f / k), s[i].vel);
}
static Vec2 calcola_coesione(const std::vector<Boid>& s, int i) {
    Vec2 somma = {0, 0};
    int k = 0;
    for (int j = 0; j < (int)s.size(); ++j) {
        if (j == i) continue;
        float d = dist(s[i].pos, s[j].pos);
        if (d > 0.0f && d < R_VIEW) { somma = add(somma, s[j].pos); k++; }
    }
    if (k == 0) return Vec2{0, 0};
    return sub(mul(somma, 1.0f / k), s[i].pos);
}

static void inizializza_stormo(std::vector<Boid>& s) {
    s.clear();
    for (int i = 0; i < N; ++i)
        s.push_back(Boid{
            { casuale(0, LARGHEZZA), casuale(0, ALTEZZA) },
            { casuale(-150, 150),    casuale(-150, 150)    }
        });
}

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 09 - Parametri");
    SetTargetFPS(60);
    srand((unsigned)time(NULL));

    std::vector<Boid> stormo;
    inizializza_stormo(stormo);

    std::vector<Vec2> sep(N), all_(N), coes(N);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // TODO 2: leggi tastiera e modifica i pesi.
        // Q/A => W_SEP, W/S => W_ALL, E/D => W_COES.
        // if (IsKeyDown(KEY_Q)) W_SEP -= 1.0f * dt;
        // if (IsKeyDown(KEY_A)) W_SEP += 1.0f * dt;
        // ... (idem per gli altri)
        // R (singolo evento) => reset.
        // if (IsKeyPressed(KEY_R)) { W_SEP = 1.5f; W_ALL = 1.0f; W_COES = 1.0f; inizializza_stormo(stormo); }

        // TODO 3: clamp >= 0.
        // if (W_SEP < 0) W_SEP = 0; (idem per gli altri)

        for (int i = 0; i < N; ++i) {
            sep[i]  = calcola_separazione(stormo, i);
            all_[i] = calcola_allineamento(stormo, i);
            coes[i] = calcola_coesione(stormo, i);
        }
        for (int i = 0; i < N; ++i) {
            Vec2 acc = add(add(mul(sep[i], W_SEP), mul(all_[i], W_ALL)), mul(coes[i], W_COES));
            stormo[i].vel = add(stormo[i].vel, mul(acc, 60.0f * dt));
            float v = norm(stormo[i].vel);
            if (v > MAX_SPEED) stormo[i].vel = mul(normalize(stormo[i].vel), MAX_SPEED);
            stormo[i].pos = add(stormo[i].pos, mul(stormo[i].vel, dt));
            stormo[i].pos = applica_wrap(stormo[i].pos);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        for (int i = 0; i < N; ++i)
            DrawCircle((int)stormo[i].pos.x, (int)stormo[i].pos.y, 3.0f, WHITE);

        // TODO 4: disegna 3 righe di testo coi valori dei pesi.
        // DrawText(TextFormat("W_SEP  = %.2f  (Q-/A+)", W_SEP),  10, 10, 18, WHITE);
        // ...

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
