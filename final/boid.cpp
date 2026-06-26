//
// Created by Marco Coppola on 22/05/2026.
//

#include "boid.hpp"
#include "danger.hpp"
#include "configs.hpp"
#include <vector>
#include <memory>
#include <random>

Boid::Boid(int id, const std::string &breed, float w_sep, float w_alig, float w_cohes, Color color, float max_speed, float r_view, float r_sep,
           float r_fear, float fear_factor, float cone_height, float cone_base_r) {
    this->color = color;
    this->max_speed = max_speed;
    this->id = id;
    this->r_fear = r_fear;
    this->fear_factor = fear_factor;
    this->breed = breed;
    this->cone_height = cone_height;
    this->cone_base_r = cone_base_r;

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

Color Boid::get_color() {
    return this->color;
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

float Boid::get_cone_base_r() const {
    return cone_base_r;
}

float Boid::get_cone_height() const {
    return cone_height;
}

void Boid::clamp_vel() {
    float current_speed_sq = norm_sq(this->vel);
    if (current_speed_sq > (this->max_speed * this->max_speed)) {
        float current_speed = std::sqrt(current_speed_sq);
        this->vel = (this->vel / current_speed) * this->max_speed;
    }
}



void Boid::evolve(const std::vector<std::unique_ptr<Boid>>& flock, Dangers* const dangers[6], float dt) {
    Vec3 sep_vector = {0.0f, 0.0f, 0.0f};
    Vec3 ali_vector = {0.0f, 0.0f, 0.0f};
    Vec3 coh_vector = {0.0f, 0.0f, 0.0f};

    int neighbor_count = 0;
    const float r_view_sq = this->r_view * this->r_view;
    const float r_sep_sq = this->r_sep * this->r_sep;

    // ----- FASE 1: un solo passaggio, accumulo direzioni/medie dei Boid -----
    for (const auto &other : flock) {
        if (this->id == other->id || this->breed != other->breed) continue;

        Vec3 offset = this->pos - other->pos;        // da "other" verso di me
        float d_sq = length_sq(offset);

        if (d_sq < r_view_sq && d_sq > 0.0f) {
            float d = std::sqrt(d_sq);
            float intensity = (this->r_view - d) / this->r_view;  // 1 vicino, 0 al bordo

            if (d_sq < r_sep_sq)                     // separazione solo entro r_sep
                sep_vector += (offset / d) * intensity;  // normalize(offset)*intensity, una sola sqrt
            ali_vector += other->vel;
            coh_vector += other->pos;
            ++neighbor_count;
        }
    }

    // ----- FASE 2: Calcolo dello steering dai pericoli (Muri/Predatori) -----
    Vec3 danger_steer = {0.0f, 0.0f, 0.0f};
    const float r_fear_sq = this->r_fear * this->r_fear;

    for (int i = 0; i < 6; ++i) {
        if (!dangers[i]) continue; // Controllo di sicurezza se il puntatore fosse nullo

        Vec3 closest_pt = dangers[i]->get_closest_point(this->pos);
        float d_sq = dist_sq(this->pos, closest_pt);

        if (d_sq > 0.0f && d_sq < r_fear_sq) {
            float d = std::sqrt(d_sq);
            Vec3 direction = normalize(this->pos - closest_pt);
            float intensity = (dangers[i]->get_base_threat() * this->fear_factor) / d;

            danger_steer += direction * intensity;
        }
    }

    // ----- FASE 3: Risoluzione delle forze dei Boid -----
    Vec3 sep = {0.0f, 0.0f, 0.0f};
    Vec3 ali = {0.0f, 0.0f, 0.0f};
    Vec3 coe = {0.0f, 0.0f, 0.0f};

    if (neighbor_count > 0) {
        const float cruise = this->max_speed;
        const float inv_count = 1.0f / static_cast<float>(neighbor_count);

        // SEPARAZIONE (desired - vel)
        if (length_sq(sep_vector) > 0.0f)
            sep = normalize(sep_vector) * cruise - this->vel;

        // ALLINEAMENTO (desired - vel)
        ali_vector *= inv_count;
        if (length_sq(ali_vector) > 0.0f)
            ali = normalize(ali_vector) * cruise - this->vel;

        // COESIONE (desired - vel)
        coh_vector = coh_vector * inv_count - this->pos;
        if (length_sq(coh_vector) > 0.0f)
            coe = normalize(coh_vector) * cruise - this->vel;
    }

    Vec3 acc =
        //sep * (this->w_sep   * Config::g_params.w_sep)
               ali * (this->w_alig  * Config::g_params.w_alig)
              + coe * (this->w_cohes * Config::g_params.w_cohes);
             // + danger_steer * Config::g_params.w_fear;
    this->pending_acc = acc;   // double-buffer: applica dopo che tutti hanno letto lo stato vecchio
}

void Boid::apply(float dt) {
    this->increment_vel(this->pending_acc * dt);
    this->increment_pos(this->get_vel() * dt);
}
