//
// Created by lucia on 22/06/2026.
//

#include "engine.hpp"

#include <iostream>
#include <raylib.h>
#include <rlgl.h>
#include "raygui.h"
#include "configs.hpp"
#include "starling.hpp"
#include "wall.hpp"
#include "utils.cpp"


void Engine::init_rl() {
    InitWindow(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT, "Storni");
    SetTargetFPS(60);
    // Fullscreen borderless: la finestra assume la risoluzione del monitor
    // (rapporto panoramico), affidabile anche su macOS.
//    ToggleBorderlessWindowed();

    // Camera dritta sull'asse Z, puntata all'origine: il cubo (centrato in 0,0,0)
    // cade esattamente al centro dell'inquadratura.
    camera.position = {0.0f, -20.0f, Config::BOX_HALF_Z + 350.0f};
    camera.target = {0.0f, -20.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;


    rlSetClipPlanes(RL_CULL_DISTANCE_NEAR, 5000.0);
}


 void Engine::build_walls() {
    dangers[0] = new Wall(Vec3{Config::BOX_HALF_X, 0, 0},   Vec3{-1, 0, 0}, Config::WALL_THREAT_FACTOR); // Right
    dangers[1] = new Wall(Vec3{-Config::BOX_HALF_X, 0, 0},  Vec3{1, 0, 0},  Config::WALL_THREAT_FACTOR); // Left
    dangers[2] = new Wall(Vec3{0, Config::BOX_HALF_Y, 0},   Vec3{0, -1, 0}, Config::WALL_THREAT_FACTOR); // Up
    dangers[3] = new Wall(Vec3{0, -Config::BOX_HALF_Y, 0},  Vec3{0, 1, 0},  Config::WALL_THREAT_FACTOR); // Down
    dangers[4] = new Wall(Vec3{0, 0, Config::BOX_HALF_Z},   Vec3{0, 0, -1}, Config::WALL_THREAT_FACTOR); // Front
    dangers[5] = new Wall(Vec3{0, 0, -Config::BOX_HALF_Z},  Vec3{0, 0, 1},  Config::WALL_THREAT_FACTOR); // Back
}

bool Engine::setup_screen() {

    // One text/value box per breed. GuiValueBox = integer entry; click to edit, Enter/click to commit.
    const float labelW = 240, w = 180, h = 44, step = 64;
    const float x = (GetScreenWidth() - (labelW + 20 + w)) / 2 + labelW + 20;

    // Larger raygui font so the menu reads big.
    GuiSetStyle(DEFAULT, TEXT_SIZE, 22);

    int  edit_breed = -1;   // which box is in edit mode (-1 = none)
    bool start = false;

    while (!start && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        GuiSetStyle(DEFAULT, TEXT_SIZE, 34);
        GuiLabel({x - labelW - 20, 90, 460, 40}, "SETUP SIMULAZIONE");
        GuiSetStyle(DEFAULT, TEXT_SIZE, 22);
        GuiLabel({x - labelW - 20, 140, 460, 30}, "Numero di boid per razza:");

        float y = 220;
        struct Row { const char *name; int *value; };
        Row rows[] = {
            {"Rondini (Swallow)",  &Config::g_spawn.swallow},
            {"Storni  (Starling)", &Config::g_spawn.starling},
            {"Gabbiani (Seagull)", &Config::g_spawn.seagull},
        };

        for (int i = 0; i < 3; ++i) {
            GuiLabel({x - labelW - 20, y + 6, labelW, h}, rows[i].name);
            if (GuiValueBox({x, y, w, h}, nullptr, rows[i].value,
                            0, Config::MAX_PER_BREED, edit_breed == i)) {
                edit_breed = (edit_breed == i) ? -1 : i;
            }
            y += step;
        }

        // Toggle rotazione orbitale della telecamera attorno al cubo.
        GuiCheckBox({x - labelW - 20, y + 6, h - 8, h - 8}, "Rotazione telecamera",
                    &Config::g_orbit_camera);
        y += step;

        GuiSetStyle(DEFAULT, TEXT_SIZE, 28);
        if (GuiButton({x - labelW - 20, y + 40, labelW + 20 + w, h + 12}, "START")) start = true;
        GuiSetStyle(DEFAULT, TEXT_SIZE, 22);

        EndDrawing();
    }

    // Restore default font size for the in-sim HUD.
    GuiSetStyle(DEFAULT, TEXT_SIZE, 10);

    auto sum = Config::g_spawn.seagull + Config::g_spawn.swallow + Config::g_spawn.starling;
    if (sum > 800) {
        std::cerr << "TROPPI";
        start = false;
    }


    return start;
}



void Engine::draw() {
    float dt = GetFrameTime();
    if (Config::g_orbit_camera) UpdateCamera(&camera, CAMERA_ORBITAL);

    // Everyone reads same global state
    for (const auto & boid : flock) {
        boid->evolve(flock, dangers, dt);
    }

    // Apply new steerings all at once
    for (const auto & boid : flock) {
        boid->apply(dt);
    }

    BeginDrawing();
    ClearBackground({ 150, 170, 200, 255 } );

    BeginMode3D(camera);
    DrawCubeWires({0, 0, 0}, 2 * Config::BOX_HALF_X, 2 * Config::BOX_HALF_Y, 2 * Config::BOX_HALF_Z,
                  DARKGRAY);
    for (const auto & boid : flock) {
        Vec3 direction = boid->get_heading();   // direzione filtrata: rotazione morbida del cono
        if (norm_sq(direction) < 0.5f) continue;
        Vector3 topPos = to_rl(boid->get_pos() + direction * boid->get_cone_height());
        DrawCylinderEx(to_rl(boid->get_pos()), topPos, boid->get_cone_base_r(), 0.0f, 12, boid->get_color());
    }


    EndMode3D();

    draw_gui();

    EndDrawing();
}

void Engine::draw_gui() {
    const float x = 14, labelW = 72, w = 190, h = 20, step = 30;
    float y = 44;

    // Pannello di sfondo scuro: stacca i controlli dal cielo chiaro.
    const float panelW = labelW + w + 48;
    const float panelH = 44 + step * 4 + 8;
    DrawRectangle(6, 6, static_cast<int>(panelW), static_cast<int>(panelH), Fade(BLACK, 0.6f));
    DrawRectangleLines(6, 6, static_cast<int>(panelW), static_cast<int>(panelH), Fade(WHITE, 0.4f));

    // raygui piu' grande e ad alto contrasto (testo chiaro su pannello scuro).
    GuiSetStyle(DEFAULT, TEXT_SIZE, 14);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(RAYWHITE));
    GuiSetStyle(SLIDER, TEXT_COLOR_NORMAL, ColorToInt(RAYWHITE));

    GuiLabel({x, 16, panelW, h}, "SIM PARAMS");

    auto slider = [&](const char* name, float* v) {
        GuiSlider({x + labelW, y, w, h}, name, TextFormat("%.2f", *v), v, 0.0f, 3.0f);
        y += step;
    };
    slider("Separaz.", &Config::g_params.w_sep);
    slider("Allinea.", &Config::g_params.w_alig);
    slider("Coesione", &Config::g_params.w_cohes);
    slider("Paura",    &Config::g_params.w_fear);

    GuiSetStyle(DEFAULT, TEXT_SIZE, 10);   // ripristina per il resto

    draw_stats();
}

// HUD in alto a destra: stato sim + metriche flock.
void Engine::draw_stats() {
    // Metriche aggregate sui boid di una data razza (match per breed, non per colore).
    // Il colore del box e' quello reale dei boid di quella razza (sempre in sync).
    struct Stats { int n = 0; float avg = 0, max = 0, order = 0; Color col = RAYWHITE; };
    auto stats_of = [&](const char* breed) {
        Stats s;
        Vec3 vel_sum{0, 0, 0};   // somma direzioni normalizzate -> ordine di Reynolds
        float speed_sum = 0.0f;
        int moving = 0;
        for (const auto& boid : flock) {
            if (boid->get_breed() != breed) continue;
            if (s.n == 0) s.col = boid->get_color();
            ++s.n;
            Vec3 v = boid->get_vel();
            float sp = norm(v);
            speed_sum += sp;
            if (sp > s.max) s.max = sp;
            if (sp > 0.0f) { vel_sum += normalize(v); ++moving; }
        }
        if (s.n) s.avg = speed_sum / s.n;
        s.order = moving ? norm(vel_sum) / moving : 0.0f;
        return s;
    };

    // Box colorato della razza con nome + statistiche, ancorato a un angolo.
    const int pw = 190, ph = 96;
    auto panel = [&](const char* name, const char* breed, int px, int py) {
        Stats s = stats_of(breed);
        Color col = s.col;
        const int hh = 26;   // altezza header

        DrawRectangle(px, py, pw, ph, Fade(BLACK, 0.6f));    // corpo scuro
        DrawRectangle(px, py, pw, hh, col);                  // header = colore razza
        DrawRectangleLines(px, py, pw, ph, col);             // bordo colore razza

        // Testo header con contrasto automatico (chiaro su scuro, scuro su chiaro).
        float lum = (0.299f * col.r + 0.587f * col.g + 0.114f * col.b) / 255.0f;
        Color head_txt = lum > 0.5f ? BLACK : RAYWHITE;

        int tx = px + 8, ty = py + 5;
        DrawText(TextFormat("%s (%d)", name, s.n), tx, ty, 18, head_txt); ty += hh + 4;
        DrawText(TextFormat("Vel media %.1f", s.avg), tx, ty, 14, RAYWHITE); ty += 18;
        DrawText(TextFormat("Vel max   %.1f", s.max), tx, ty, 14, RAYWHITE); ty += 18;
        DrawText(TextFormat("Ordine    %.2f", s.order), tx, ty, 14, RAYWHITE);
    };

    const int W = GetScreenWidth(), H = GetScreenHeight(), m = 10;

    // FPS in alto al centro.
    DrawText(TextFormat("FPS %d", GetFPS()), W / 2 - 40, m, 18,
             GetFPS() >= 55 ? GREEN : (GetFPS() >= 30 ? YELLOW : RED));

    // Un box per razza in tre angoli (in alto a sinistra ci sono gli slider).
    panel("Rondini",  "swallow",  W - pw - m, m);          // ALTO DX
    panel("Storni",   "starling", m, H - ph - m);          // BASSO SX
    panel("Gabbiani", "seagull",  W - pw - m, H - ph - m); // BASSO DX
}

void Engine::clean() {
    CloseWindow();
}
