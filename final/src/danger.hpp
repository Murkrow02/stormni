#ifndef GAME_DANGER_H
#define GAME_DANGER_H
#include "vec3.hpp"
#include <raylib.h>

namespace sim {

class Danger {
  public:
    virtual ~Danger() = default;
    virtual float get_base_threat() const = 0;
    virtual Vec3 get_closest_point(const Vec3& boid_pos) const = 0;
};

} // namespace sim

#endif // GAME_DANGER_H
