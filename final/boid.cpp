//
// Created by Marco Coppola on 22/05/2026.
//

#include "boid.hpp"
#include "danger.hpp"
#include "configs.hpp"
#include <vector>
#include <memory>
#include <random>

namespace sim {

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
    this->w_sep = w_sep;
    this->w_alig = w_alig;
    this->w_cohes = w_cohes;
    this->r_view = r_view;
    this->r_sep = r_sep;

    // Tutti i boid sono identici: l'unica casualita' e' posizione e direzione iniziale.
    static std::random_device rd;
    static std::default_random_engine eng(rd());

    std::uniform_real_distribution<float> dist_x(-BOX_HALF_X, BOX_HALF_X);
    std::uniform_real_distribution<float> dist_y(-BOX_HALF_Y, BOX_HALF_Y);
    std::uniform_real_distribution<float> dist_z(-BOX_HALF_Z, BOX_HALF_Z);
    this->pos = {dist_x(eng), dist_y(eng), dist_z(eng)};

    // direzione casuale, velocita' iniziale = max_speed (gia' a regime, niente transitori)
    std::uniform_real_distribution<float> dist_dir(-1.0f, 1.0f);
    this->vel = normalize(Vec3{dist_dir(eng), dist_dir(eng), dist_dir(eng)}) * this->max_speed;
    this->heading = normalize(this->vel);
}

void Boid::apply_wrap() {
    if (this->pos.x < -BOX_HALF_X)
        this->pos.x += 2 * BOX_HALF_X;
    if (this->pos.x > BOX_HALF_X)
        this->pos.x -= 2 * BOX_HALF_X;
    if (this->pos.y < -BOX_HALF_Y)
        this->pos.y += 2 * BOX_HALF_Y;
    if (this->pos.y > BOX_HALF_Y)
        this->pos.y -= 2 * BOX_HALF_Y;
    if (this->pos.z < -BOX_HALF_Z)
        this->pos.z += 2 * BOX_HALF_Z;
    if (this->pos.z > BOX_HALF_Z)
        this->pos.z -= 2 * BOX_HALF_Z;
}

Vec3 Boid::get_vel() const {
    return vel;
}

Vec3 Boid::get_heading() const {
    return heading;
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

const std::string& Boid::get_breed() const {
    return this->breed;
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



// Steering classico di Reynolds: forza = limit(normalize(desired)*max_speed - vel, max_force).
// Restituisce 0 se 'desired' e' nullo (nessun vicino / direzione indefinita).
Vec3 Boid::steer(Vec3 desired, float max_force) const {
    if (length_sq(desired) <= 0.0f) return {0.0f, 0.0f, 0.0f};
    Vec3 target_vel = normalize(desired) * this->max_speed;
    return limit(target_vel - this->vel, max_force);
}

void Boid::evolve(const std::vector<std::unique_ptr<Boid>>& flock, Danger* const dangers[6], float dt) {
    Vec3 sep_sum = {0.0f, 0.0f, 0.0f};   // somma direzioni di fuga, pesate per 1/d
    Vec3 ali_sum = {0.0f, 0.0f, 0.0f};   // somma velocita' dei vicini
    Vec3 coh_sum = {0.0f, 0.0f, 0.0f};   // somma posizioni dei vicini (per il centroide)

    int view_count = 0;   // vicini entro r_view  (allineamento + coesione)
    int sep_count  = 0;   // vicini entro r_sep   (separazione)

    const float r_view_sq = this->r_view * this->r_view;
    const float r_sep_sq  = this->r_sep  * this->r_sep;

    // ----- FASE 1: un solo passaggio, accumulo le medie dai vicini -----
    for (const auto &other : flock) {
        if (this->id == other->id) continue;

        Vec3 offset = this->pos - other->pos;   // da "other" verso di me
        float d_sq = length_sq(offset);
        if (d_sq <= 0.0f) continue;

        // Separazione: contro TUTTI i boid, anche di razza diversa (evita collisioni tra specie)
        if (d_sq < r_sep_sq) {
            float d = std::sqrt(d_sq);
            sep_sum += (offset / d) / d;        // normalize(offset) / d, piu' vicino = repulsione piu' forte
            ++sep_count;
        }

        // Allineamento + coesione: solo con la stessa razza
        if (this->breed == other->breed && d_sq < r_view_sq) {
            ali_sum += other->vel;
            coh_sum += other->pos;
            ++view_count;
        }
    }

    // Forza di sterzo massima (limite di Reynolds): tiene le virate fluide e stabili.
    const float max_force = 4.0f * this->max_speed;

    // ----- FASE 2: tre regole, ognuna come forza di sterzo limitata -----
    Vec3 sep{0.0f, 0.0f, 0.0f}, ali{0.0f, 0.0f, 0.0f}, coe{0.0f, 0.0f, 0.0f};

    if (sep_count > 0)
        sep = steer(sep_sum, max_force);                       // allontanati dai vicini

    if (view_count > 0) {
        const float inv_count = 1.0f / static_cast<float>(view_count);
        ali = steer(ali_sum * inv_count, max_force);           // verso la velocita' media
        Vec3 to_centroid = coh_sum * inv_count - this->pos;
        coe = steer(to_centroid, max_force);                   // verso il centroide
    }

    // ----- FASE 3: steering dai pericoli (Muri + boid-pericolo come i gabbiani) -----
    Vec3 danger_steer = {0.0f, 0.0f, 0.0f};
    const float r_fear_sq = this->r_fear * this->r_fear;

    // Fuga generica da un Dangers: piu' vicino = repulsione piu' forte.
    auto flee = [&](const Danger* dgr) {
        Vec3 closest_pt = dgr->get_closest_point(this->pos);
        float d_sq = dist_sq(this->pos, closest_pt);
        if (d_sq > 0.0f && d_sq < r_fear_sq) {
            float d = std::sqrt(d_sq);
            danger_steer += normalize(this->pos - closest_pt)
                          * ((dgr->get_base_threat() * this->fear_factor) / d);
        }
    };

    // Pericoli statici (muri).
    for (int i = 0; i < 6; ++i)
        if (dangers[i]) flee(dangers[i]);

    for (const auto &other : flock) {
        if (this->id == other->id || this->breed == other->breed) continue;
        if (const Danger* dgr = dynamic_cast<const Danger*>(other.get()))
            flee(dgr);
    }

    // ----- FASE 4: somma pesata + limite totale sull'accelerazione -----
    Vec3 acc = sep * (this->w_sep   * g_params.w_sep)
             + ali * (this->w_alig  * g_params.w_alig)
             + coe * (this->w_cohes * g_params.w_cohes)
           + danger_steer * g_params.w_fear;

    this->pending_acc = limit(acc, max_force);   // double-buffer: applica dopo che tutti leggono lo stato vecchio
    (void)dt;
}

void Boid::apply(float dt) {
    this->increment_vel(this->pending_acc * dt);
    this->increment_pos(this->get_vel() * dt);

    // Heading filtrato: easing esponenziale verso la direzione della velocita'.
    // alpha indipendente dal frame-rate; TURN_RATE alto = piu' reattivo, basso = piu' morbido.
    constexpr float TURN_RATE = 4.0f;
    float alpha = 1.0f - std::exp(-TURN_RATE * dt);
    Vec3 target_dir = normalize(this->vel);
    if (length_sq(target_dir) > 0.0f) {
        this->heading = this->heading + (target_dir - this->heading) * alpha;
        this->heading = normalize(this->heading);
    }
}

} // namespace sim
