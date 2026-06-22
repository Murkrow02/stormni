#include <iostream>
#include <memory>
#include "configs.hpp"
#include "boid.hpp"
#include "raylib.h"
#include "starling.hpp"
#include "wall.hpp"
#include "utils.cpp"


int main() {

    // Init RL

    // engine = new Engine();
    // engine->initRL();
    InitWindow(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, "Storni");
    SetTargetFPS(60);
    Camera3D camera = {0};
    camera.position = {0.0f, 250.0f, 500.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;


    // Fill flock with starlings
    // engine->fillFlock();
    std::vector<Boid> flock;
    for (int i = 0; i < Config::N; ++i) {
        flock.push_back(Starling(i));
    }

    float L = Config::BOX_HALF_EXTENT;
    float WALL_THREAT = 100.0f;

    // Fill dangers
    Dangers* dangers[6];
    dangers[0] = new Wall(Vec3{L, 0, 0},   Vec3{-1, 0, 0}, WALL_THREAT); // Right
    dangers[1] = new Wall(Vec3{-L, 0, 0},  Vec3{1, 0, 0},  WALL_THREAT); // Left
    dangers[2] = new Wall(Vec3{0, L, 0},   Vec3{0, -1, 0}, WALL_THREAT); // Up
    dangers[3] = new Wall(Vec3{0, -L, 0},  Vec3{0, 1, 0},  WALL_THREAT); // Down
    dangers[4] = new Wall(Vec3{0, 0, L},   Vec3{0, 0, -1}, WALL_THREAT); // Front
    dangers[5] = new Wall(Vec3{0, 0, -L},  Vec3{0, 0, 1},  WALL_THREAT); // Back


    while (!WindowShouldClose()) {

        // engine->evolve()
        float dt = GetFrameTime();
        UpdateCamera(&camera, CAMERA_ORBITAL);

        // Cycle on each boid
        for (int i = 0; i < Config::N; ++i) {

            // Calc all steerings
            flock[i].evolve(flock, dangers, dt);
        }

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);
        DrawCubeWires({0, 0, 0}, 2 * Config::BOX_HALF_EXTENT, 2 * Config::BOX_HALF_EXTENT, 2 * Config::BOX_HALF_EXTENT,
                      DARKGRAY);
        for (int i = 0; i < Config::N; ++i) {


            // 2. Definisci le dimensioni del cono
            float coneBaseRadius = 1.5f; // Raggio della base del boid
            float coneHeight = 5.0f;     // Lunghezza del boid

            // 3. Calcola il punto finale (la punta del cono) spingendoti lungo la velocità
            Vec3 direction = normalize(flock[i].get_vel());
            Vector3 topPos = to_rl(flock[i].get_pos() + direction * coneHeight);

            // 4. Disegna il cono usando la funzione cilindro estesa
            // startPos = currentPos (base)
            // endPos = topPos (punta)
            // startRadius = coneBaseRadius (base larga)
            // endRadius = 0.0f (diventa un cono perfetto)
            // sides = 12 (risoluzione 3D)
            DrawCylinderEx(to_rl(flock[i].get_pos()), topPos, coneBaseRadius, 0.0f, 12, WHITE);
        }


        EndMode3D();
        // DrawText(TextFormat("W_SEP  = %.2f  (Q+/A-)", W_SEP), 10, 10, 18, WHITE);
        // DrawText(TextFormat("W_ALL  = %.2f  (W+/S-)", W_ALL), 10, 30, 18, WHITE);
        // DrawText(TextFormat("W_COES  = %.2f  (E+/D-)", W_COES), 10, 50, 18, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
