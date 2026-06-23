#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <vector>
#include <memory>
#include "boid.hpp"
#include "configs.hpp"
#include "danger.hpp"


class Engine {
    Dangers* dangers[6];
    Camera3D camera = {0};
    std::vector<std::unique_ptr<Boid>> flock;

public:
    void init_rl();
    void build_walls();
    void draw();
    void clean();

    template<typename T>
    void fill_flock() {
        for (int i = 0; i < Config::N; ++i) {
            flock.push_back(std::make_unique<T>(i));
        }
    }
};

#endif //GAME_ENGINE_H