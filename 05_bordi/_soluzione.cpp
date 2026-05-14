// Soluzione step 05.
#include "raylib.h"
#include "vec2.h"
#include <vector>
#include <cstdlib>
#include <ctime>

const int LARGHEZZA = 800;
const int ALTEZZA   = 600;
const int N         = 100;

struct Boid { Vec2 pos; Vec2 vel; };

static float casuale(float a, float b) {
    return a + ((float)rand() / RAND_MAX) * (b - a);
}

// Applica il wrap toroidale a una posizione.
static Vec2 applica_wrap(Vec2 p) {
    if (p.x < 0)         p.x += LARGHEZZA;
    if (p.x > LARGHEZZA) p.x -= LARGHEZZA;
    if (p.y < 0)         p.y += ALTEZZA;
    if (p.y > ALTEZZA)   p.y -= ALTEZZA;
    return p;
}

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 05 - Bordi");
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

        for (int i = 0; i < N; ++i) {
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
