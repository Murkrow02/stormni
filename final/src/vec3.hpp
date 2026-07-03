#ifndef GAME_VEC3_H
#define GAME_VEC3_H
#include <cmath>

namespace sim {

struct Vec3 {

    float x, y, z;

    inline Vec3& operator+=(Vec3 const& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    inline Vec3& operator*=(float k) {
        x *= k;
        y *= k;
        z *= k;
        return *this;
    }

    inline Vec3 operator-() const { return {-x, -y, -z}; }

    inline Vec3 operator+(Vec3 const& v) const {
        auto result = *this;
        return result += v;
    }

    inline Vec3 operator-(Vec3 const& v) const { return {x - v.x, y - v.y, z - v.z}; }

    inline Vec3 operator*(float k) const {
        auto result = *this;
        return result *= k;
    }

    inline Vec3 operator/(float k) const { return *this * (1 / k); }

    inline bool operator==(Vec3 const& v) const { return x == v.x && y == v.y && z == v.z; }

    inline bool operator!=(Vec3 const& v) const { return !(*this == v); }
};

inline static float norm(Vec3 a) {
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}
inline static float norm_sq(Vec3 a) {
    return (a.x * a.x + a.y * a.y + a.z * a.z);
}
static inline Vec3 normalize(Vec3 a) {
    float n = norm(a);
    if (n > 0.0f) return Vec3{a / n};
    return Vec3{0, 0, 0};
}

// Tronca la lunghezza del vettore a 'max' mantenendone la direzione
static inline Vec3 limit(Vec3 v, float max) {
    float m2 = norm_sq(v);
    if (m2 > max * max && m2 > 0.0f) return v * (max / sqrtf(m2));
    return v;
}

inline float dot(Vec3 const& a, Vec3 const& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline float dist_sq(Vec3 a, Vec3 b) {
    return norm_sq(a - b);
}
inline float dist(Vec3 a, Vec3 b) {
    return norm(a - b);
}

// Alias for above
inline float length(Vec3 a) {
    return norm(a);
}
inline float length_sq(Vec3 a) {
    return norm_sq(a);
}

} // namespace sim

#endif // GAME_VEC3_H