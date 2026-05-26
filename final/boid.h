#pragma once
#include <random>
#include <raylib.h>

#include "vec3.h"

class Boid {

    Vec3 pos;
    Vec3 vel;
    Color color;

    float w_sep, w_all, w_coes;
    
    std::default_random_engine gen;


public:
    Boid();
    Boid(Vec3 pos);

};



#endif //GAME_BOID_H
