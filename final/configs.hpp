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
    const float WALL_THREAT_FACTOR = 500.0f;

    struct SimParams {
        float w_sep   = 1.0f;
        float w_alig  = 1.0f;
        float w_cohes = 1.0f;
        float w_fear  = 1.0f;
    };

    inline SimParams g_params;

}
#endif //GAME_CONFIGS_H