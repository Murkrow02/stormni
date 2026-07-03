#include "configs.hpp"
#include "engine.hpp"
#include "raylib.h"
#include "seagull.hpp"
#include "starling.hpp"
#include "swallow.hpp"

using namespace sim;

int main() {

    auto engine = new Engine();
    engine->init_rl();

    // Setup
    if (!engine->setup_screen()) {
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

    delete engine;

    return 0;
}
