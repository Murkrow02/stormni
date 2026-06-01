#ifndef BOID_HPP
#define BOID_HPP
#include <raylib.h>
#include <memory>
#include <vector>
#include "vec3.hpp"

class Dangers;
class Boid {
    int id;

    void clamp_vel();

    void apply_wrap();

protected:
    Vec3 pos;
    Vec3 vel;
    float max_speed, r_view, r_sep, r_fear, fear_factor;
    //fear factor da 0 a 1, che poi moltiplica il threat del repulsore?!
    Color color;

    float w_sep, w_alig, w_cohes;

public:
    Boid(int id, float w_sep, float w_alig, float w_cohes, Color color, float max_speed, float r_view, float r_sep,
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

    void increment_pos(Vec3);

    float get_r_fear() const;

    float get_fear_factor() const;

    /*----------------------
     * REYNOLD'S RULES
     ---------------------*/

    Vec3 separation_from(const std::vector<Boid> &flock) const;
    Vec3 alignment_to(const std::vector<Boid> &flock) const;
    Vec3 cohesion_with(const std::vector<Boid> &flock) const;

    Vec3 flee_from(const std::vector<std::unique_ptr<Dangers> > &pericoli) const;
};


#endif
