#ifndef BOID_HPP
#define BOID_HPP
#include <random>
#include <raylib.h>

#include "vec3.hpp"

class Boid {
    int id;
    void clamp_vel();
    void apply_wrap();

protected:
    Vec3 pos;
    Vec3 vel;
    float max_speed, r_view, r_sep;
    Color color;

    float w_sep, w_alig, w_cohes;

public:
    Boid(int id, float w_sep, float w_alig, float w_cohes, Color color, float max_speed, float r_view, float r_sep);


    /*----------------------
     * GET & SET
     ---------------------*/
    Vec3 get_vel() const;
    void set_vel(Vec3);
    void increment_vel(Vec3);

    Vec3 get_pos() const;
    void set_pos(Vec3);
    void increment_pos(Vec3);

    /*----------------------
     * REYNOLD'S RULES
     ---------------------*/
    Vec3 separation_from(const std::vector<Boid> &flock) const {
        Vec3 steer = {0, 0, 0};

        for (const auto &i: flock) {
            if (this->id == i.id) continue;
            float d = dist(this->pos, i.pos);
            if (d > 0.0f && d < r_sep)
                steer += normalize(this->pos - i.pos) / d;
        }
        return steer;
    }

    Vec3 alignment_to(const std::vector<Boid> &flock) const {
        Vec3 sum = {0, 0, 0};
        int k = 0;
        for (const auto &i: flock) {
            if (i.id == this->id);
            float d = dist(i.pos, this->pos);
            if (d > 0.0f && d < r_view) {
                sum += i.vel;
                k++;
            }
        }
        if (k == 0) return Vec3{0, 0, 0};
        return (sum / k) - this->vel;
    }

    Vec3 cohesion_with(const std::vector<Boid> &flock) const {
        Vec3 sum = {0, 0, 0};
        int k = 0;
        for (const auto &i: flock) {
            if (i.id == this->id) continue;
            float d = dist(this->pos, i.pos);
            if (d > 0.0f && d < r_view) {
                sum += i.pos;
                k++;
            }
        }
        if (k == 0) return Vec3{0, 0, 0};
        return (sum / k) - this->pos;
    }
};


#endif
