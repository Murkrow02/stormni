#ifndef GAME_WALL_H
#define GAME_WALL_H
#include "danger.hpp"
#include <stdexcept>

namespace sim {

class Wall : public Danger {
    Vec3 origin;
    Vec3 normal;
    float threat;

  public:
    Wall(Vec3 o, Vec3 n, float threat) {
        this->origin = o;
        this->normal = normalize(n);
        this->threat = threat;

        if (norm_sq(n) == 0.0f) {
            throw std::runtime_error{"The normal vector of the Wall cannot be a zero vector"};
        }
        if (threat < 0.0f) {
            throw std::runtime_error{"The Wall's threat factor cannot be negative"};
        }
    }
    float get_base_threat() const override { return threat; }

    // VEDI MATEMATICA
    Vec3 get_closest_point(const Vec3& boid_pos) const override {
        float min_dist = dot(boid_pos - origin, normal);
        return boid_pos - (normal * min_dist);
    }
};

} // namespace sim

#endif // GAME_WALL_H
