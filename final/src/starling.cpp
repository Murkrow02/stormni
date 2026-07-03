#include "starling.hpp"

namespace sim {

Starling::Starling(int id)
    : Boid(id, "starling", 1, 1, 1, DARKBROWN, 65, 60, 30, 60, 1, 16.0f, 5.0f) {}

} // namespace sim
