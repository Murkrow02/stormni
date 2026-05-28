//
// Created by lucia on 28/05/2026.
//

#ifndef GAME_REPULSORS_H
#define GAME_REPULSORS_H
#include "Vec3.hpp"
#include <raylib.h>

class Boid;

class Dangers {
protected:
    Color color;
    float base_threat;

public:
    // distruttore va messo??
    // virtual ~Dangers() = default;
    float get_base_threat() const {
        return base_threat;
    }
    virtual Vec3 get_closest_point(const Vec3& boid_pos) const = 0;
    virtual void movement(float dt){}

    //così va bene per ogg puntiformi o sferici, per il muro???

};

#endif //GAME_REPULSORS_H
