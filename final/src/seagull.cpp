#include "seagull.hpp"
#include "configs.hpp"

namespace sim {

Seagull::Seagull(int id) : Boid(id, "seagull", 1, 1, 1, WHITE, 60, 60, 30, 20, 1, 16.0f, 5.0f) {}

float Seagull::get_base_threat() const {
    return SEAGULL_THREAT_FACTOR;
}

Vec3 Seagull::get_closest_point(const Vec3& /*boid_pos*/) const {
    return this->get_pos();
}

} // namespace sim
