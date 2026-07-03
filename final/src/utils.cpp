#include "vec3.hpp"
#include <raylib.h>
namespace sim {
static Vector3 to_rl(Vec3 v) {
    return Vector3{v.x, v.y, v.z};
}
} // namespace sim