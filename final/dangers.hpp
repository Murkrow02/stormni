//
// Created by lucia on 28/05/2026.
//

#ifndef GAME_REPULSORS_H
#define GAME_REPULSORS_H
#include "Vec3.hpp"
#include <raylib.h>

class Boid;

class Repulsors {
protected:
    Vec3 pos;
    Vec3 vel;
    Color color;
    float base_threat;

public:
    float get_base_threat() const {
        return base_threat;
    }
    Vec3 get_pos() const {
        return pos;
    }
    virtual void movement(float dt){}

    //così va bene per ogg puntiformi o sferici, per il muro???

};

#endif //GAME_REPULSORS_H
