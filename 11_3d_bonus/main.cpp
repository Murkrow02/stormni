// Step 11 — Murmuration in 3D.

#include "raylib.h"
#include "vec3.h"
#include <vector>
#include <cstdlib>
#include <ctime>

const int LARGHEZZA = 800;
const int ALTEZZA   = 600;
const int N         = 100;

const float BOX       = 200.0f;     // metà lato del cubo dello spazio simulato
const float R_SEP     = 20.0f;
const float R_VIEW    = 50.0f;
const float MAX_SPEED = 200.0f;
const float W_SEP     = 1.5f;
const float W_ALL     = 1.0f;
const float W_COES    = 1.0f;

struct Boid { Vec3 pos; Vec3 vel; };

static float casuale(float a, float b) {
    return a + ((float)rand() / RAND_MAX) * (b - a);
}

// TODO 3: wrap toroidale 3D (tre componenti, range [-BOX, +BOX]).
// Vec3 applica_wrap(Vec3 p) {
//     if (p.x < -BOX) p.x += 2*BOX;  if (p.x > BOX) p.x -= 2*BOX;
//     if (p.y < -BOX) p.y += 2*BOX;  if (p.y > BOX) p.y -= 2*BOX;
//     if (p.z < -BOX) p.z += 2*BOX;  if (p.z > BOX) p.z -= 2*BOX;
//     return p;
// }

// TODO 1: implementa le 3 regole con Vec3 (sono identiche allo step 08, basta cambiare tipo).
// static Vec3 calcola_separazione(const std::vector<Boid>& s, int i) { ... }
// static Vec3 calcola_allineamento(const std::vector<Boid>& s, int i) { ... }
// static Vec3 calcola_coesione   (const std::vector<Boid>& s, int i) { ... }

// Converte la nostra Vec3 al Vector3 di raylib (stessa struttura, tipi diversi).
static Vector3 to_rl(Vec3 v) { return Vector3{ v.x, v.y, v.z }; }

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 11 - 3D");
    SetTargetFPS(60);
    srand((unsigned)time(NULL));

    // TODO 2: inizializza N boid con pos casuale in [-BOX, BOX] su 3 assi.
    std::vector<Boid> stormo;
    // for (int i = 0; i < N; ++i) ... stormo.push_back(...)

    std::vector<Vec3> sep(N), all_(N), coes(N);

    // Camera 3D.
    Camera3D camera = {0};
    camera.position   = { 0.0f, 250.0f, 500.0f };
    camera.target     = { 0.0f, 0.0f,   0.0f   };
    camera.up         = { 0.0f, 1.0f,   0.0f   };
    camera.fovy       = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // TODO 5: ruota la camera attorno allo stormo.
        // UpdateCamera(&camera, CAMERA_ORBITAL);

        // Fisica (uguale al 2D, ma Vec3).
        // for (int i = 0; i < N; ++i) { ... }

        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(camera);
            DrawCubeWires({0, 0, 0}, 2*BOX, 2*BOX, 2*BOX, DARKGRAY);
            // TODO 4: disegna ogni boid come sfera.
            // for (int i = 0; i < N; ++i)
            //     DrawSphere(to_rl(stormo[i].pos), 2.0f, WHITE);
        EndMode3D();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
