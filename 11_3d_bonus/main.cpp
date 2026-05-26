// Step 11 — Murmuration in 3D.

#include "raylib.h"
#include "vec3.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>

const int LARGHEZZA = 800;
const int ALTEZZA = 600;
const int N = 600;

const float BOX = 200.0f; // metà lato del cubo dello spazio simulato
const float R_SEP = 40.0f;
const float R_VIEW = 50.0f;
const float MAX_SPEED = 200.0f;


struct Boid {
    Vec3 pos;
    Vec3 vel;
};



std::default_random_engine gen;

static float random(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}


Vec3 applica_wrap(Vec3 p) {
    if (p.x < -BOX)
        p.x += 2 * BOX;
    if (p.x > BOX)
        p.x -= 2 * BOX;
    if (p.y < -BOX)
        p.y += 2 * BOX;
    if (p.y > BOX)
        p.y -= 2 * BOX;
    if (p.z < -BOX)
        p.z += 2 * BOX;
    if (p.z > BOX)
        p.z -= 2 * BOX;
    return p;
}


static Vec3 calcola_separazione(const std::vector<Boid> &s, int i) {
    Vec3 spinta = {0, 0, 0};

    for (int j = 0; j < (int) s.size(); ++j) {
        if (j == i) continue;
        float d = dist(s[i].pos, s[j].pos);
        if (d > 0.0f && d < R_SEP)
            spinta = add(spinta, mul(normalize(sub(s[i].pos, s[j].pos)), 1.0f / d));
    }
    return spinta;
}

static Vec3 calcola_allineamento(const std::vector<Boid>& s, int i) {
    Vec3 somma = {0,0,0}; int k = 0;
    for (int j = 0; j < (int)s.size(); ++j) {
        if (j == i) continue;
        float d = dist(s[i].pos, s[j].pos);
        if (d > 0.0f && d < R_VIEW) { somma = add(somma, s[j].vel); k++; }
    }
    if (k == 0) return Vec3{0,0,0};
    return sub(mul(somma, 1.0f / k), s[i].vel);
}

static Vec3 calcola_coesione(const std::vector<Boid> &s, int i) {
    Vec3 somma = {0, 0, 0};
    int k = 0;
    for (int j = 0; j < (int) s.size(); ++j) {
        if (j == i) continue;
        float d = dist(s[i].pos, s[j].pos);
        if (d > 0.0f && d < R_VIEW) {
            somma = add(somma, s[j].pos);
            k++;
        }
    }
    if (k == 0) return Vec3{0, 0, 0};
    return sub(mul(somma, 1.0f / k), s[i].pos);
}

// Converte la nostra Vec3 al Vector3 di raylib (stessa struttura, tipi diversi).
static Vector3 to_rl(Vec3 v) { return Vector3{v.x, v.y, v.z}; }

int main() {
    InitWindow(LARGHEZZA, ALTEZZA, "Storni - Step 11 - 3D");
    SetTargetFPS(60);
    srand((unsigned) time(NULL));


    std::vector<Boid> stormo;
    for (int i = 0; i < N; ++i) {
        stormo.push_back(Boid{
            {random(-BOX, BOX), random(-BOX, BOX), random(-BOX, BOX)},
            {random(-150, 150), random(-150, 150), random(-150, 150)},

        });
    }

    std::vector<Vec3> sep(N), all_(N), coes(N);

    // Camera 3D.
    Camera3D camera = {0};
    camera.position = {0.0f, 250.0f, 500.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float W_SEP = 5.5f;
    float W_ALL = 1.0f;
    float W_COES = 1.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();



        UpdateCamera(&camera, CAMERA_ORBITAL);
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

        for (int i = 0; i < N; ++i) {
            sep[i] = calcola_separazione(stormo, i);
            all_[i] = calcola_allineamento(stormo, i);
            coes[i] = calcola_coesione(stormo, i);

            Vec3 acc = add(add(mul(sep[i], W_SEP), mul(all_[i], W_ALL)), mul(coes[i], W_COES));
            stormo[i].vel = add(stormo[i].vel, mul(acc, dt));
            stormo[i].vel.x = std::min(stormo[i].vel.x, MAX_SPEED);
            stormo[i].vel.y = std::min(stormo[i].vel.y, MAX_SPEED);
            stormo[i].vel.z = std::min(stormo[i].vel.z, MAX_SPEED);
            stormo[i].pos = add(stormo[i].pos, mul(stormo[i].vel, dt));
            stormo[i].pos = applica_wrap(stormo[i].pos);
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);

        BeginMode3D(camera);
        DrawCubeWires({0, 0, 0}, 2 * BOX, 2 * BOX, 2 * BOX, DARKGRAY);
        for (int i = 0; i < N; ++i)
            DrawSphere(to_rl(stormo[i].pos), 2.0f, WHITE);
        EndMode3D();
        DrawText(TextFormat("W_SEP  = %.2f  (Q+/A-)", W_SEP), 10, 10, 18, WHITE);
        DrawText(TextFormat("W_ALL  = %.2f  (W+/S-)", W_ALL), 10, 30, 18, WHITE);
        DrawText(TextFormat("W_COES  = %.2f  (E+/D-)", W_COES), 10, 50, 18, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
