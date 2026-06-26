//
// Created by lucia on 28/05/2026.
//

#ifndef GAME_REPULSORS_H
#define GAME_REPULSORS_H
#include "vec3.hpp"
#include <raylib.h>

namespace sim {

class Danger {
public:
    virtual ~Danger() = default;
    virtual float get_base_threat() const = 0;
    virtual Vec3 get_closest_point(const Vec3& boid_pos) const = 0;
};

}

#endif //GAME_REPULSORS_H
