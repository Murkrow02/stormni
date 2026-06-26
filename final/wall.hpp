//
// Created by lucia on 01/06/2026.
//

#ifndef GAME_WALL_H
#define GAME_WALL_H
#include "danger.hpp"

namespace sim {

class Wall : public Danger {
    Vec3 origin;
    Vec3 normal;
    float threat;

public:
    Wall(Vec3 o, Vec3 n, float threat) {
        this->origin = o;
        this->normal = normalize(n);
        this->threat = threat;
    }
    float get_base_threat() const override {
        return threat;
    }
    Vec3 get_closest_point(const Vec3& boid_pos) const override {
        float min_dist = dot(boid_pos - origin, normal);
        return boid_pos - (normal * min_dist);
    }
};

} // namespace sim

#endif //GAME_WALL_H
