#include <raylib.h>

#include "vec3.hpp"
//
// Created by lucia on 22/06/2026.
//
namespace sim {
static Vector3 to_rl(Vec3 v) { return Vector3{v.x, v.y, v.z}; }
} // namespace sim