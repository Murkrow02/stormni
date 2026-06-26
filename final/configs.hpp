#ifndef GAME_CONFIGS_H
#define GAME_CONFIGS_H
#include <string_view>

namespace Config {

    // Window params
    const int WINDOW_HEIGHT = 800;
    const int WINDOW_WIDTH = 600;

    // Viewport params
    const float BOX_HALF_EXTENT = 200.0f;

    // Simulation params
    const int N = 100;
    const float WALL_THREAT_FACTOR = 100.0f;
}
#endif //GAME_CONFIGS_H