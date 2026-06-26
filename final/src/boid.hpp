#ifndef BOID_HPP
#define BOID_HPP
#include <raylib.h>
#include <memory>
#include <string>
#include <vector>
#include "vec3.hpp"

namespace sim {

class Danger;

class Boid {
    int id;
    std::string breed;
    void clamp_vel();
    void apply_wrap();
    Vec3 steer(Vec3 desired, float max_force) const;

protected:
    Vec3 pos;
    Vec3 vel;
    Vec3 heading{0.0f, 0.0f, 0.0f};
    Vec3 pending_acc{0.0f, 0.0f, 0.0f};
    float max_speed, r_view, r_sep, r_fear, fear_factor;
    Color color;
    float cone_height, cone_base_r;
    float w_sep, w_alig, w_cohes;

public:
    Boid(int id, const std::string &breed, float w_sep, float w_alig, float w_cohes, Color color, float max_speed, float r_view, float r_sep,
         float r_fear, float fear_factor, float cone_height, float cone_base_r);

    virtual ~Boid() = default;

    /*----------------------
     * GET & SET
     ---------------------*/
    Vec3 get_vel() const;

    Vec3 get_heading() const;

    void set_vel(Vec3);

    void increment_vel(Vec3);

    Vec3 get_pos() const;

    void set_pos(Vec3);

    Color get_color();

    const std::string& get_breed() const;

    void increment_pos(Vec3);

    float get_r_fear() const;

    float get_fear_factor() const;

    float get_cone_height() const;
    float get_cone_base_r() const;

    void evolve(const std::vector<std::unique_ptr<Boid>>& boids, Danger* const walls[6], float dt);

    void apply(float dt);
};

} // namespace sim

#endif
