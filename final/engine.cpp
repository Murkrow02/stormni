//
// Created by lucia on 22/06/2026.
//

#include "engine.hpp"
#include <raylib.h>
#include "raygui.h"
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
    //UpdateCamera(&camera, CAMERA_ORBITAL);

    // Everyone reads same global state
    for (const auto & boid : flock) {
        boid->evolve(flock, dangers, dt);
    }

    // Apply new steerings all at once
    for (const auto & boid : flock) {
        boid->apply(dt);
    }

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode3D(camera);
    DrawCubeWires({0, 0, 0}, 2 * Config::BOX_HALF_EXTENT, 2 * Config::BOX_HALF_EXTENT, 2 * Config::BOX_HALF_EXTENT,
                  DARKGRAY);
    for (const auto & boid : flock) {
        Vec3 direction = normalize(boid->get_vel());
        Vector3 topPos = to_rl(boid->get_pos() + direction * boid->get_cone_height());
        DrawCylinderEx(to_rl(boid->get_pos()), topPos, boid->get_cone_base_r(), 0.0f, 12, boid->get_color());
    }


    EndMode3D();

    draw_gui();

    EndDrawing();
}

void Engine::draw_gui() {
    const float x = 10, w = 180, h = 16, step = 26;
    float y = 20;

    GuiLabel({x, 4, w, h}, "SIM PARAMS");

    GuiSlider({x + 70, y, w, h}, "Separaz.", TextFormat("%.2f", Config::g_params.w_sep),
              &Config::g_params.w_sep, 0.0f, 3.0f);  y += step;
    GuiSlider({x + 70, y, w, h}, "Allinea.", TextFormat("%.2f", Config::g_params.w_alig),
              &Config::g_params.w_alig, 0.0f, 3.0f);  y += step;
    GuiSlider({x + 70, y, w, h}, "Coesione", TextFormat("%.2f", Config::g_params.w_cohes),
              &Config::g_params.w_cohes, 0.0f, 3.0f);  y += step;
    GuiSlider({x + 70, y, w, h}, "Paura",    TextFormat("%.2f", Config::g_params.w_fear),
              &Config::g_params.w_fear, 0.0f, 3.0f);
}

void Engine::clean() {
    CloseWindow();
}
