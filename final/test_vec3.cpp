#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "vec3.hpp"

// Tolleranza per i confronti tra float
static constexpr float kEps = 1e-4f;

TEST_CASE("operatori aritmetici di Vec3") {
    Vec3 a{1.0f, 2.0f, 3.0f};
    Vec3 b{4.0f, -1.0f, 0.5f};

    SUBCASE("somma") {
        Vec3 s = a + b;
        CHECK(s.x == doctest::Approx(5.0f));
        CHECK(s.y == doctest::Approx(1.0f));
        CHECK(s.z == doctest::Approx(3.5f));
    }

    SUBCASE("differenza") {
        Vec3 d = a - b;
        CHECK(d.x == doctest::Approx(-3.0f));
        CHECK(d.y == doctest::Approx(3.0f));
        CHECK(d.z == doctest::Approx(2.5f));
    }

    SUBCASE("negazione") {
        Vec3 n = -a;
        CHECK(n.x == doctest::Approx(-1.0f));
        CHECK(n.y == doctest::Approx(-2.0f));
        CHECK(n.z == doctest::Approx(-3.0f));
    }

    SUBCASE("prodotto per scalare") {
        Vec3 m = a * 2.0f;
        CHECK(m.x == doctest::Approx(2.0f));
        CHECK(m.y == doctest::Approx(4.0f));
        CHECK(m.z == doctest::Approx(6.0f));
    }

    SUBCASE("divisione per scalare") {
        Vec3 q = a / 2.0f;
        CHECK(q.x == doctest::Approx(0.5f));
        CHECK(q.y == doctest::Approx(1.0f));
        CHECK(q.z == doctest::Approx(1.5f));
    }

    SUBCASE("operatori composti +=  e  *=") {
        Vec3 c = a;
        c += b;
        CHECK(c == a + b);
        Vec3 e = a;
        e *= 3.0f;
        CHECK(e == a * 3.0f);
    }
}

TEST_CASE("uguaglianza tra Vec3") {
    Vec3 a{1.0f, 2.0f, 3.0f};
    CHECK(a == Vec3{1.0f, 2.0f, 3.0f});
    CHECK(a != Vec3{1.0f, 2.0f, 3.5f});
    CHECK_FALSE(a != a);
}

TEST_CASE("norma e norma al quadrato") {
    Vec3 v{3.0f, 4.0f, 0.0f};
    CHECK(norm(v) == doctest::Approx(5.0f));
    CHECK(norm_sq(v) == doctest::Approx(25.0f));
    CHECK(length(v) == doctest::Approx(norm(v)));
    CHECK(length_sq(v) == doctest::Approx(norm_sq(v)));

    Vec3 zero{0.0f, 0.0f, 0.0f};
    CHECK(norm(zero) == doctest::Approx(0.0f));
}

TEST_CASE("normalize") {
    SUBCASE("vettore non nullo ha norma 1 e mantiene la direzione") {
        Vec3 v{0.0f, 5.0f, 0.0f};
        Vec3 n = normalize(v);
        CHECK(norm(n) == doctest::Approx(1.0f));
        CHECK(n == Vec3{0.0f, 1.0f, 0.0f});
    }

    SUBCASE("il vettore nullo resta nullo (nessuna divisione per zero)") {
        Vec3 n = normalize(Vec3{0.0f, 0.0f, 0.0f});
        CHECK(n == Vec3{0.0f, 0.0f, 0.0f});
    }

    SUBCASE("direzione preservata per vettore generico") {
        Vec3 v{1.0f, 2.0f, 2.0f};   // norma 3
        Vec3 n = normalize(v);
        CHECK(norm(n) == doctest::Approx(1.0f));
        CHECK(n.x == doctest::Approx(1.0f / 3.0f));
        CHECK(n.y == doctest::Approx(2.0f / 3.0f));
        CHECK(n.z == doctest::Approx(2.0f / 3.0f));
    }
}

TEST_CASE("limit tronca la lunghezza mantenendo la direzione") {
    Vec3 v{3.0f, 4.0f, 0.0f};   // norma 5

    SUBCASE("oltre il massimo: viene troncato") {
        Vec3 r = limit(v, 1.0f);
        CHECK(norm(r) == doctest::Approx(1.0f));
        CHECK(normalize(r).x == doctest::Approx(0.6f));
        CHECK(normalize(r).y == doctest::Approx(0.8f));
    }

    SUBCASE("entro il massimo: invariato") {
        Vec3 r = limit(v, 10.0f);
        CHECK(r == v);
    }

    SUBCASE("vettore nullo: invariato, nessuna divisione per zero") {
        Vec3 r = limit(Vec3{0.0f, 0.0f, 0.0f}, 5.0f);
        CHECK(r == Vec3{0.0f, 0.0f, 0.0f});
    }
}

TEST_CASE("dot product") {
    CHECK(dot(Vec3{1.0f, 0.0f, 0.0f}, Vec3{0.0f, 1.0f, 0.0f}) == doctest::Approx(0.0f));
    CHECK(dot(Vec3{1.0f, 2.0f, 3.0f}, Vec3{4.0f, 5.0f, 6.0f}) == doctest::Approx(32.0f));
    Vec3 v{2.0f, -1.0f, 3.0f};
    CHECK(dot(v, v) == doctest::Approx(norm_sq(v)));   // dot(v,v) == |v|^2
}

TEST_CASE("dist e dist_sq") {
    Vec3 a{0.0f, 0.0f, 0.0f};
    Vec3 b{3.0f, 4.0f, 0.0f};
    CHECK(dist(a, b) == doctest::Approx(5.0f));
    CHECK(dist_sq(a, b) == doctest::Approx(25.0f));
    CHECK(dist(a, a) == doctest::Approx(0.0f));
    CHECK(dist(a, b) == doctest::Approx(dist(b, a)));   // simmetria
    (void)kEps;
}
