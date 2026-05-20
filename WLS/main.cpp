
#include "raylib.h"
#include "vec2.hpp"
#include <vector>
#include <random>
#include <ctime>

const int LENGHT = 800;
const int HEIGHT = 600;
const int N = 100;
const int boid_radius = 3;
const float R_VIEW = 50.0f;
const float K_AL = 1.0f;
const float R_SEP = 20.0f;
const float MAX_SPEED = 200.0f;
const float K_SE = 250.0f;

struct Boid
{
    Vec2 pos;
    Vec2 vel;
};

std::default_random_engine gen;
static float random(float min, float max)
{
    std::uniform_real_distribution<float> dist(min,max);
    return dist(gen);
   
}

static Vec2 respawn(Vec2 p)
{
    if (p.x <= 0 - boid_radius)
    {
        p.x += LENGHT + boid_radius;
    }

    if (p.x >= LENGHT + boid_radius)
    {
        p.x -= LENGHT - boid_radius;
    }
    if (p.y <= 0 - boid_radius)
    {
        p.y += HEIGHT + boid_radius;
    }

    if (p.y >= HEIGHT + boid_radius)
    {
        p.y -= HEIGHT - boid_radius;
    }
    return p;
}


Vec2 cal_steer_se(const std::vector<Boid> &stormo, int i)
{
    Vec2 steer{0, 0};
    for (int j = 0; j != N; ++j)
    {
        if (i == j)
            continue;
        float d = norm(sub(stormo[i].pos, stormo[j].pos));
        if (d <= R_SEP)
        {
            Vec2 v_dis = sub(stormo[i].pos, stormo[j].pos);
            steer = add(steer, mul(v_dis, 1.0f / d));
            continue;
        }
    }
    return steer;
}

Vec2 cal_steer_al(const std::vector<Boid> &stormo, int i)
{
    Vec2 v_sum{0, 0};
    float tot = 0;
    for (int j = 0; j != N; ++j)
    {
        if (j == i)
            continue;
        float d = dist(stormo[i].pos, stormo[j].pos);
        if (d <= R_VIEW)
        {
            v_sum = add(mul(normalize(v_sum), MAX_SPEED), stormo[j].vel);
            tot += 1;
        }
    }
    if (tot == 0)
    {
        return v_sum;
    }
    Vec2 v_cm = mul(v_sum, 1 / tot);
    Vec2 steer_all = sub(v_cm, stormo[i].vel);
    return steer_all;
}

int main()
{
    InitWindow(LENGHT, HEIGHT, "Storni - Step 06 - Separazione");
    SetTargetFPS(60);
    srand((unsigned)time(NULL));

    std::vector<Boid> stormo;
    for (int i = 0; i < N; ++i)
    {
        stormo.push_back(Boid{
            {random(0, LENGHT), random(0, HEIGHT)},
            {random(-150, 150), random(-150, 150)}});
    }

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        for (int i = 0; i < N; ++i)
        {
            Vec2 steer_se{0, 0};
            Vec2 steer_al{0, 0};
            steer_se = cal_steer_se(stormo, i);
            steer_al = cal_steer_al(stormo, i);
            stormo[i].vel = add(stormo[i].vel, add(mul(steer_se, K_SE * dt), mul(steer_al, K_AL * dt)));
            stormo[i].vel.x = std::min(stormo[i].vel.x, MAX_SPEED);
            stormo[i].vel.y = std::min(stormo[i].vel.y, MAX_SPEED);
            stormo[i].pos = add(stormo[i].pos, mul(stormo[i].vel, dt));
            stormo[i].pos = respawn(stormo[i].pos);
        }

        BeginDrawing();
        ClearBackground(BLACK);
        for (int i = 0; i < N; ++i)
        {
            DrawCircle(stormo[i].pos.x, stormo[i].pos.y, R_SEP, CLITERAL(Color){255, 0, 0, 100});
            DrawCircle(stormo[i].pos.x, stormo[i].pos.y, boid_radius, WHITE);
            DrawCircle(stormo[i].pos.x, stormo[i].pos.y, R_VIEW, CLITERAL(Color){0, 200, 0, 70});
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
