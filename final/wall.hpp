//
// Created by lucia on 01/06/2026.
//

#ifndef GAME_WALL_H
#define GAME_WALL_H
#include "danger.hpp"

class Wall : public Dangers {
    Vec3 origin;
    Vec3 normal;

public:
    Wall(Vec3 o, Vec3 n, float threat) {
        this->origin = o;
        this->normal = normalize(n);
        this->base_threat = threat;
    }
    virtual Vec3 get_closest_point(const Vec3& boid_pos) const override {
        float min_dist = dot(boid_pos - origin, normal);
        return boid_pos - (normal * min_dist);
    }
};

#endif //GAME_WALL_H
