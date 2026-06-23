#ifndef BOID_HPP
#define BOID_HPP
#include <raylib.h>
#include <memory>
#include <string>
#include <vector>
#include "vec3.hpp"

class Dangers;

class Boid {
    int id;
    std::string breed;
    void clamp_vel();

    void apply_wrap();

protected:
    Vec3 pos;
    Vec3 vel;
    Vec3 pending_acc{0.0f, 0.0f, 0.0f};
    float max_speed, r_view, r_sep, r_fear, fear_factor;
    //fear factor da 0 a 1, che poi moltiplica il threat del repulsore?!
    Color color;

    float w_sep, w_alig, w_cohes;

public:
    Boid(int id, const std::string &breed, float w_sep, float w_alig, float w_cohes, Color color, float max_speed, float r_view, float r_sep,
         float r_fear, float fear_factor);

    virtual ~Boid() = default;

    /*----------------------
     * GET & SET
     ---------------------*/
    Vec3 get_vel() const;

    void set_vel(Vec3);

    void increment_vel(Vec3);

    Vec3 get_pos() const;

    void set_pos(Vec3);

    Color get_color();

    void increment_pos(Vec3);

    float get_r_fear() const;

    float get_fear_factor() const;

    void evolve(const std::vector<std::unique_ptr<Boid>>& flock, Dangers* const dangers[6], float dt);

    void apply(float dt);
};

#endif
