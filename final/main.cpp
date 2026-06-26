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
#include "seagull.hpp"

using namespace sim;

int main() {


    auto engine = new Engine();
    engine->init_rl();

    // Ask the user how many of each breed to spawn before building the flock.
    if (!engine->setup_screen()) {
        engine->clean();
        delete engine;
        return 0;
    }

    engine->fill_flock<Swallow>(g_spawn.swallow);
    engine->fill_flock<Starling>(g_spawn.starling);
    engine->fill_flock<Seagull>(g_spawn.seagull);

    engine->build_walls();

    while (!WindowShouldClose()) {
        engine->draw();
    }

    engine->clean();

    return 0;
}
