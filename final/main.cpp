#include <iostream>
#include <memory>
#include "configs.hpp"
#include "boid.hpp"
#include "raylib.h"
#include "starling.hpp"
#include "wall.hpp"

static Vector3 to_rl(Vec3 v) { return Vector3{v.x, v.y, v.z}; }

int main() {
    InitWindow(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, "Storni");
    SetTargetFPS(60);

    std::vector<Boid> flock;
    for (int i = 0; i < Config::N; ++i) {
        flock.push_back(Starling(i));
    }
    std::vector<std::unique_ptr<Dangers>> pericoli;
    float L = Config::BOX_HALF_EXTENT;
    float WALL_THREAT = 100.0f;
    pericoli.push_back(std::make_unique<Wall>(Vec3{L, 0, 0}, Vec3{-1, 0, 0}, WALL_THREAT));  // Right
    pericoli.push_back(std::make_unique<Wall>(Vec3{-L, 0, 0}, Vec3{1, 0, 0}, WALL_THREAT)); // Left
    pericoli.push_back(std::make_unique<Wall>(Vec3{0, L, 0}, Vec3{0, -1, 0}, WALL_THREAT));  // Up
    pericoli.push_back(std::make_unique<Wall>(Vec3{0, -L, 0}, Vec3{0, 1, 0}, WALL_THREAT)); // Down
    pericoli.push_back(std::make_unique<Wall>(Vec3{0, 0, L}, Vec3{0, 0, -1}, WALL_THREAT));  // Front
    pericoli.push_back(std::make_unique<Wall>(Vec3{0, 0, -L}, Vec3{0, 0, 1}, WALL_THREAT)); // Back

    std::vector<Vec3> sep(Config::N), all_(Config::N), coes(Config::N);

    // Camera 3D.
    Camera3D camera = {0};
    camera.position = {0.0f, 250.0f, 500.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    //perchè sono ancora qui?
    float W_SEP = 5.5f;
    float W_ALL = 1.0f;
    float W_COES = 1.0f;
    float W_FLEE = 1.5f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();



        //UpdateCamera(&camera, CAMERA_ORBITAL);
        //no coefficienti negativi
        if (IsKeyDown(KEY_Q)) W_SEP += 10.0F *dt;
        if (IsKeyDown(KEY_W)) W_ALL += 10.0F *dt;
        if (IsKeyDown(KEY_E)) W_COES += 10.0F *dt;
        if (IsKeyDown(KEY_A)) W_SEP -= 10.0F *dt;
        if (IsKeyDown(KEY_S)) W_ALL -= 10.0F *dt;
        if (IsKeyDown(KEY_D)) W_COES -= 10.0F *dt;
        if (IsKeyDown(KEY_R)) {
            W_SEP = 1.0F;
            W_ALL = 1.0F;
            W_COES = 1.0F;
        }

        for (int i = 0; i < Config::N; ++i) {
            sep[i] = flock[i].separation_from(flock);
            all_[i] = flock[i].alignment_to(flock);
            coes[i] = flock[i].cohesion_with(flock);

            Vec3 flee = flock[i].flee_from(pericoli);

            Vec3 acc = sep[i] * W_SEP + all_[i] * W_ALL + coes[i] * W_COES + flee *W_FLEE;
            flock[i].increment_vel(acc * dt);
            flock[i].increment_pos(flock[i].get_vel() * dt);

        }

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);
        DrawCubeWires({0, 0, 0}, 2 * Config::BOX_HALF_EXTENT, 2 *  Config::BOX_HALF_EXTENT, 2 *  Config::BOX_HALF_EXTENT, DARKGRAY);
        for (int i = 0; i < Config::N; ++i) {
            DrawSphere(to_rl(flock[i].get_pos()), 2.0f, WHITE);
            continue;
        }
        EndMode3D();
        DrawText(TextFormat("W_SEP  = %.2f  (Q+/A-)", W_SEP), 10, 10, 18, WHITE);
        DrawText(TextFormat("W_ALL  = %.2f  (W+/S-)", W_ALL), 10, 30, 18, WHITE);
        DrawText(TextFormat("W_COES  = %.2f  (E+/D-)", W_COES), 10, 50, 18, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}