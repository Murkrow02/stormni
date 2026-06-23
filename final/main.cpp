#include <iostream>
#include <memory>
#include "configs.hpp"
#include "boid.hpp"
#include "raylib.h"
#include "starling.hpp"
#include "wall.hpp"
#include "utils.cpp"
#include "engine.hpp"
#include "swallow.hpp"

int main() {


    auto engine = new Engine();
    engine->init_rl();
    engine->fill_flock<Swallow>();
    engine->fill_flock<Starling>();
    engine->build_walls();

    while (!WindowShouldClose()) {
        engine->draw();
    }

    engine->clean();

    return 0;
}
