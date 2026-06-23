//
// Created by lucia on 22/06/2026.
//

#include "engine.hpp"
#include <raylib.h>
#include "configs.hpp"
#include "starling.hpp"
#include "wall.hpp"
#include "utils.cpp"

void Engine::init_rl() {
    InitWindow(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, "Storni");
    SetTargetFPS(60);
    camera.position = {0.0f, 250.0f, 500.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}


 void Engine::build_walls() {
    dangers[0] = new Wall(Vec3{Config::BOX_HALF_EXTENT, 0, 0},   Vec3{-1, 0, 0}, Config::WALL_THREAT_FACTOR); // Right
    dangers[1] = new Wall(Vec3{-Config::BOX_HALF_EXTENT, 0, 0},  Vec3{1, 0, 0},  Config::WALL_THREAT_FACTOR); // Left
    dangers[2] = new Wall(Vec3{0, Config::BOX_HALF_EXTENT, 0},   Vec3{0, -1, 0}, Config::WALL_THREAT_FACTOR); // Up
    dangers[3] = new Wall(Vec3{0, -Config::BOX_HALF_EXTENT, 0},  Vec3{0, 1, 0},  Config::WALL_THREAT_FACTOR); // Down
    dangers[4] = new Wall(Vec3{0, 0, Config::BOX_HALF_EXTENT},   Vec3{0, 0, -1}, Config::WALL_THREAT_FACTOR); // Front
    dangers[5] = new Wall(Vec3{0, 0, -Config::BOX_HALF_EXTENT},  Vec3{0, 0, 1},  Config::WALL_THREAT_FACTOR); // Back
}

void Engine::draw() {
    float dt = GetFrameTime();
    UpdateCamera(&camera, CAMERA_ORBITAL);

    for (const auto & boid : flock) {
        boid->evolve(flock, dangers, dt);
    }

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(camera);
    DrawCubeWires({0, 0, 0}, 2 * Config::BOX_HALF_EXTENT, 2 * Config::BOX_HALF_EXTENT, 2 * Config::BOX_HALF_EXTENT,
                  DARKGRAY);
    for (const auto & boid : flock) {
        float coneBaseRadius = 1.5f;
        float coneHeight = 5.0f;
        Vec3 direction = normalize(boid->get_vel());
        Vector3 topPos = to_rl(boid->get_pos() + direction * coneHeight);
        DrawCylinderEx(to_rl(boid->get_pos()), topPos, coneBaseRadius, 0.0f, 12, boid->get_color());
    }


    EndMode3D();
    // DrawText(TextFormat("W_SEP  = %.2f  (Q+/A-)", W_SEP), 10, 10, 18, WHITE);
    // DrawText(TextFormat("W_ALL  = %.2f  (W+/S-)", W_ALL), 10, 30, 18, WHITE);
    // DrawText(TextFormat("W_COES  = %.2f  (E+/D-)", W_COES), 10, 50, 18, WHITE);

    EndDrawing();
}

void Engine::clean() {
    CloseWindow();
}
