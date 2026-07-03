#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "boid.hpp"
#include "danger.hpp"
#include <memory>
#include <vector>

namespace sim {

class Engine {
    Danger* dangers[6] = {nullptr};
    Camera3D camera = {0};
    std::vector<std::unique_ptr<Boid>> boids;
    int next_id = 0; // unique across all breeds so separation skips only self

  public:
    ~Engine();
    void init_rl();
    void build_walls();
    bool setup_screen(); // raygui menu: per-breed spawn counts; false if window closed
    void draw();
    void draw_gui();
    void draw_stats(); // HUD: FPS, conteggi, metriche flock

    template <typename T> void fill_flock(int count) {
        for (int i = 0; i < count; ++i) {
            boids.push_back(std::make_unique<T>(next_id++));
        }
    }
};

} // namespace sim

#endif // GAME_ENGINE_H