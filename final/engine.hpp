#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <vector>
#include <memory>
#include "boid.hpp"
#include "configs.hpp"
#include "danger.hpp"


class Engine {
    Danger* dangers[6];
    Camera3D camera = {0};
    std::vector<std::unique_ptr<Boid>> flock;
    int next_id = 0;   // unique across all breeds so separation skips only self

public:
    void init_rl();
    void build_walls();
    bool setup_screen();   // raygui menu: per-breed spawn counts; false if window closed
    void draw();
    void draw_gui();
    void draw_stats();   // HUD: FPS, conteggi, metriche flock
    void clean();

    template<typename T>
    void fill_flock(int count) {
        for (int i = 0; i < count; ++i) {
            flock.push_back(std::make_unique<T>(next_id++));
        }
    }
};

#endif //GAME_ENGINE_H