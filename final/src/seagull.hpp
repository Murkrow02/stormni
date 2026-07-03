#ifndef GAME_SEAGULL_H
#define GAME_SEAGULL_H
#include "boid.hpp"
#include "danger.hpp"

namespace sim {

class Seagull : public Boid, public Danger {

  public:
    Seagull(int id);
    float get_base_threat() const override;
    Vec3 get_closest_point(const Vec3& boid_pos) const override;
};

} // namespace sim

#endif // GAME_SEAGULL_H
