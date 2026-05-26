//
// Created by Marco Coppola on 22/05/2026.
//

#include "boid.hpp"

#include "configs.hpp"


static float casuale(float a, float b) {
    return a + ((float)rand() / RAND_MAX) * (b - a);
}

Boid::Boid(int id, float w_sep, float w_alig, float w_cohes, Color color, float max_speed, float r_view, float r_sep) {

    this->color = color;
    this->max_speed = max_speed;
    this->id = id;

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
    this->r_sep= r_sep + dist_radius_noise(eng);

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

void Boid::set_vel(const Vec3 vel)  {
    this->vel = vel;
    clamp_vel();
}

void Boid::increment_vel(const Vec3 vel)  {
    this->vel += vel;
    clamp_vel();
}

Vec3 Boid::get_pos() const {
    return pos;
}

void Boid::set_pos(const Vec3 pos)  {
    this->pos = pos;
    apply_wrap();
}

void Boid::increment_pos(const Vec3 pos)  {
    this->pos += pos;
    apply_wrap();
}


void Boid::clamp_vel() {
    this->vel.x = std::min(this->vel.x, this->max_speed);
    this->vel.y = std::min(this->vel.y, this->max_speed);
    this->vel.z = std::min(this->vel.z, this->max_speed);
}
