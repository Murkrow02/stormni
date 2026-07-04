#include "configs.hpp"
#include "engine.hpp"
#include "raylib.h"
#include "seagull.hpp"
#include "starling.hpp"
#include "swallow.hpp"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

using namespace sim;

int main() {
    try {
        Engine engine;
        engine.init_rl();

        // Setup
        if (!engine.setup_screen()) {
            return EXIT_SUCCESS;
        }

        engine.fill_flock<Swallow>(g_spawn.swallow);
        engine.fill_flock<Starling>(g_spawn.starling);
        engine.fill_flock<Seagull>(g_spawn.seagull);

        engine.build_walls();

        while (!WindowShouldClose()) {
            engine.draw();
        }

        return EXIT_SUCCESS;

    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return EXIT_FAILURE;
    }
}