#ifndef GAME_CONFIGS_H
#define GAME_CONFIGS_H
#include <string_view>

namespace sim {

// Window params: dimensioni iniziali, prima del passaggio a fullscreen.
const int WINDOW_HEIGHT = 780;
const int WINDOW_WIDTH = 1320;

// Box di simulazione: parallelepipedo, Z dimezzato rispetto a X/Y.
const float BOX_HALF = 160.0f;
const float BOX_HALF_X = BOX_HALF * 1.5;
const float BOX_HALF_Y = BOX_HALF;
const float BOX_HALF_Z = BOX_HALF / 2.0f;

// Simulation params
const int N = 100;
const float WALL_THREAT_FACTOR = 500.0f;
const float SEAGULL_THREAT_FACTOR = 1500.0f;

struct SimParams {
    float w_sep = 1.5f;
    float w_alig = 1.0f;
    float w_cohes = 1.0f;
    float w_fear = 1.0f;
};

inline SimParams g_params;

// How many boids of each breed to spawn (set on the setup screen).
struct SpawnCounts {
    int swallow = 100;
    int starling = 100;
    int seagull = 100;
};

inline SpawnCounts g_spawn;

// Rotazione orbitale della telecamera attorno al cubo (toggle nel menu iniziale).
inline bool g_orbit_camera = false;

const int MAX_PER_BREED = 1000;

} // namespace sim
#endif // GAME_CONFIGS_H