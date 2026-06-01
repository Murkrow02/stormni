//
// Created by Marco Coppola on 22/05/2026.
//

#include "boid.hpp"
#include "dangers.hpp"
#include "configs.hpp"
#include <vector>
#include <memory>
#include <random>

Boid::Boid(int id, float w_sep, float w_alig, float w_cohes, Color color, float max_speed, float r_view, float r_sep,
           float r_fear, float fear_factor) {
    this->color = color;
    this->max_speed = max_speed;
    this->id = id;
    this->r_fear = r_fear;
    this->fear_factor = fear_factor;

    // Add some noise to parameters to make simulation look more organic
    static std::random_device rd;
    static std::default_random_engine eng(rd());

    std::uniform_real_distribution<float> dist_w_noise(-1, 1);
    this->w_sep = w_sep + dist_w_noise(eng);
    this->w_alig = w_alig + dist_w_noise(eng);
    this->w_cohes = w_cohes + dist_w_noise(eng);

    std::uniform_real_distribution<float> dist_max_speed_noise(-40, 40);
    this->max_speed = max_speed + dist_max_speed_noise(eng);

    std::uniform_real_distribution<float> dist_vel_noise(-150, 150);
    this->vel = {dist_vel_noise(eng), dist_vel_noise(eng), dist_vel_noise(eng)};

    std::uniform_real_distribution<float> dist_radius_noise(-20, 20);
    this->r_view = r_view + dist_radius_noise(eng);
    this->r_sep = r_sep + dist_radius_noise(eng);

    std::uniform_real_distribution<float> dist_pos(-Config::BOX_HALF_EXTENT, Config::BOX_HALF_EXTENT);
    this->pos = {dist_pos(eng), dist_pos(eng), dist_pos(eng)};
}

void Boid::apply_wrap() {
    if (this->pos.x < -Config::BOX_HALF_EXTENT)
        this->pos.x += 2 * Config::BOX_HALF_EXTENT;
    if (this->pos.x > Config::BOX_HALF_EXTENT)
        this->pos.x -= 2 * Config::BOX_HALF_EXTENT;
    if (this->pos.y < -Config::BOX_HALF_EXTENT)
        this->pos.y += 2 * Config::BOX_HALF_EXTENT;
    if (this->pos.y > Config::BOX_HALF_EXTENT)
        this->pos.y -= 2 * Config::BOX_HALF_EXTENT;
    if (this->pos.z < -Config::BOX_HALF_EXTENT)
        this->pos.z += 2 * Config::BOX_HALF_EXTENT;
    if (this->pos.z > Config::BOX_HALF_EXTENT)
        this->pos.z -= 2 * Config::BOX_HALF_EXTENT;
}

Vec3 Boid::get_vel() const {
    return vel;
}

void Boid::set_vel(const Vec3 vel) {
    this->vel = vel;
    clamp_vel();
}

void Boid::increment_vel(const Vec3 vel) {
    this->vel += vel;
    clamp_vel();
}

Vec3 Boid::get_pos() const {
    return pos;
}

void Boid::set_pos(const Vec3 pos) {
    this->pos = pos;
    apply_wrap();
}

void Boid::increment_pos(const Vec3 pos) {
    this->pos += pos;
    apply_wrap();
}

float Boid::get_r_fear() const {
    return r_fear;
}

float Boid::get_fear_factor() const {
    return fear_factor;
}

void Boid::clamp_vel() {
    this->vel.x = std::min(this->vel.x, this->max_speed);
    this->vel.y = std::min(this->vel.y, this->max_speed);
    this->vel.z = std::min(this->vel.z, this->max_speed);
}

Vec3 Boid::separation_from(const std::vector<Boid> &flock) const {
    Vec3 steer = {0, 0, 0};

    for (const auto &i: flock) {
        if (this->id == i.id) continue;
        float d = dist(this->pos, i.pos);
        if (d > 0.0f && d < r_sep)
            steer += normalize(this->pos - i.pos) / d;
    }
    return steer;
}

Vec3 Boid::alignment_to(const std::vector<Boid> &flock) const {
    Vec3 sum = {0, 0, 0};
    int k = 0;
    for (const auto &i: flock) {
        if (i.id == this->id) continue;
        float d = dist(i.pos, this->pos);
        if (d > 0.0f && d < r_view) {
            sum += i.vel;
            k++;
        }
    }
    if (k == 0) return Vec3{0, 0, 0};
    return (sum / k) - this->vel;
}
Vec3 Boid::cohesion_with(const std::vector<Boid> &flock) const {
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

//calcolare repulsione da ostacoli-predatori
Vec3 Boid::flee_from(const std::vector<std::unique_ptr<Dangers>> &pericoli) const {
    Vec3 steer = {0, 0, 0};

    for (const auto &pericolo: pericoli) {
        Vec3 closest_pt = pericolo->get_closest_point(this->pos);
        float d = dist(this->pos, closest_pt);

        if (d > 0.0f && d < this->r_fear) {
            Vec3 direction = normalize(this->pos - closest_pt);
            float intensity = (pericolo->get_base_threat() * this->fear_factor) / d;

            steer += direction * intensity;
        }
    }
    return steer;
}
