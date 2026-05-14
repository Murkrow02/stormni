// Soluzione step 04.
#include "raylib.h"
#include "vec2.h"
#include <vector>
#include <cstdlib>
#include <ctime>

const int LARGHEZZA = 800;
const int ALTEZZA   = 600;
const int N         = 100;

struct Boid {
    Vec2 pos;
    Vec2 vel;
};

// Helper: float casuale in [a, b].
static float casuale(float a, float b) {
    float r = (float)rand() / RAND_MAX;
    return a + r * (b - a);
}

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 04 - Stormo");
    SetTargetFPS(60);
    srand((unsigned)time(NULL));

    std::vector<Boid> stormo;
    stormo.reserve(N);
    for (int i = 0; i < N; ++i) {
        Boid b;
        b.pos = { casuale(0, LARGHEZZA), casuale(0, ALTEZZA) };
        b.vel = { casuale(-150, 150),    casuale(-150, 150)    };
        stormo.push_back(b);
    }

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        for (int i = 0; i < N; ++i) {
            stormo[i].pos = add(stormo[i].pos, mul(stormo[i].vel, dt));
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
