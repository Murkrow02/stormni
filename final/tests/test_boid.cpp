#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "boid.hpp"
#include "configs.hpp"
#include "danger.hpp"
#include "doctest.h"
#include "starling.hpp"
#include "swallow.hpp"
#include "vec3.hpp"
#include "wall.hpp"
#include <memory>
#include <vector>

using namespace sim;

static constexpr float K_MAX_SPEED = 105.0f;
static constexpr float K_R_VIEW = 60.0f;
static constexpr float K_R_SEP = 30.0f;

static Danger* no_dangers[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

// Performs a simulation step just as the Engine does:
static void step(std::vector<std::unique_ptr<Boid>>& flock, float dt) {
    for (auto& b : flock)
        b->evolve(flock, no_dangers, dt);
    for (auto& b : flock)
        b->apply(dt);
}

TEST_CASE("position and velocity set/get") {
    Swallow b(0);
    b.set_pos(Vec3{10.0f, -20.0f, 30.0f});
    CHECK(b.get_pos() == Vec3{10.0f, -20.0f, 30.0f});
    b.set_vel(Vec3{1.0f, 2.0f, 3.0f});
    CHECK(b.get_vel() == Vec3{1.0f, 2.0f, 3.0f});
}

TEST_CASE("clamp velocity to max_speed") {
    Swallow b(0);

    SUBCASE("excessive velocity is clamped to max_speed") {
        b.set_vel(Vec3{1000.0f, 0.0f, 0.0f});
        CHECK(norm(b.get_vel()) == doctest::Approx(K_MAX_SPEED));
        CHECK(b.get_vel().x == doctest::Approx(K_MAX_SPEED)); 
    }

    SUBCASE("velocity within the limit stays unchanged") {
        Vec3 v{3.0f, 4.0f, 0.0f}; 
        b.set_vel(v);
        CHECK(b.get_vel() == v);
    }
}

TEST_CASE("toroidal position wrap at the rectangular box edges") {
    const float X = BOX_HALF_X;
    const float Y = BOX_HALF_Y;
    Swallow b(0);

    SUBCASE("past the positive x edge re-enters from the opposite side") {
        b.set_pos(Vec3{X + 50.0f, 0.0f, 0.0f});
        CHECK(b.get_pos().x == doctest::Approx(-X + 50.0f));
    }

    SUBCASE("past the negative y edge re-enters from the opposite side") {
        b.set_pos(Vec3{0.0f, -Y - 30.0f, 0.0f});
        CHECK(b.get_pos().y == doctest::Approx(Y - 30.0f));
    }

    SUBCASE("inside the box stays unchanged") {
        Vec3 p{10.0f, -10.0f, 5.0f};
        b.set_pos(p);
        CHECK(b.get_pos() == p);
    }
}

TEST_CASE("separation: two boids too close move apart") {
    std::vector<std::unique_ptr<Boid>> flock;
    flock.push_back(std::make_unique<Swallow>(0));
    flock.push_back(std::make_unique<Swallow>(1));
    Boid* a = flock[0].get();
    Boid* b = flock[1].get();

    const float close = K_R_SEP * 0.3f; 
    a->set_pos(Vec3{0.0f, 0.0f, 0.0f});
    b->set_pos(Vec3{close, 0.0f, 0.0f});
    a->set_vel(Vec3{0.0f, 0.0f, 0.0f});
    b->set_vel(Vec3{0.0f, 0.0f, 0.0f});

    const float before = dist(a->get_pos(), b->get_pos());
    step(flock, 0.1f);
    const float after = dist(a->get_pos(), b->get_pos());

    CHECK(after > before);        // distance increases
    CHECK(a->get_pos().x < 0.0f); 
    CHECK(b->get_pos().x > 0.0f); 
}

TEST_CASE("cohesion: a boid steers toward a still neighbor of the same species") {
    std::vector<std::unique_ptr<Boid>> flock;
    flock.push_back(std::make_unique<Swallow>(0));
    flock.push_back(std::make_unique<Swallow>(1));
    Boid* a = flock[0].get();
    Boid* b = flock[1].get();

    // Beyond r_sep but within r_view
    const float mid = (K_R_SEP + K_R_VIEW) * 0.5f;
    a->set_pos(Vec3{0.0f, 0.0f, 0.0f});
    b->set_pos(Vec3{mid, 0.0f, 0.0f});
    a->set_vel(Vec3{0.0f, 0.0f, 0.0f});
    b->set_vel(Vec3{0.0f, 0.0f, 0.0f});

    step(flock, 0.05f);

    CHECK(a->get_vel().x > 0.0f);
}

TEST_CASE("alignment: a boid matches its direction to the neighbor's") {
    const float saved_cohes = g_params.w_cohes;
    g_params.w_cohes = 0.0f; // overlooking cohesion

    std::vector<std::unique_ptr<Boid>> flock;
    flock.push_back(std::make_unique<Swallow>(0));
    flock.push_back(std::make_unique<Swallow>(1));
    Boid* a = flock[0].get();
    Boid* b = flock[1].get();

    const float mid = (K_R_SEP + K_R_VIEW) * 0.5f;
    a->set_pos(Vec3{0.0f, 0.0f, 0.0f});
    b->set_pos(Vec3{0.0f, mid, 0.0f}); 
    a->set_vel(Vec3{0.0f, 0.0f, 0.0f});
    b->set_vel(Vec3{K_MAX_SPEED, 0.0f, 0.0f}); 

    step(flock, 0.05f);

    CHECK(a->get_vel().x > 0.0f); 

    g_params.w_cohes = saved_cohes;
}

TEST_CASE("separation between different species: happens anyway") {
    std::vector<std::unique_ptr<Boid>> flock;
    flock.push_back(std::make_unique<Swallow>(0));
    flock.push_back(std::make_unique<Starling>(1));
    Boid* a = flock[0].get();
    Boid* b = flock[1].get();

    const float close = K_R_SEP * 0.3f;
    a->set_pos(Vec3{0.0f, 0.0f, 0.0f});
    b->set_pos(Vec3{close, 0.0f, 0.0f});
    a->set_vel(Vec3{0.0f, 0.0f, 0.0f});
    b->set_vel(Vec3{0.0f, 0.0f, 0.0f});

    step(flock, 0.1f);

    CHECK(a->get_pos().x < 0.0f); 
    CHECK(b->get_pos().x > 0.0f);
}

TEST_CASE("no alignment between different species") {
    std::vector<std::unique_ptr<Boid>> flock;
    flock.push_back(std::make_unique<Swallow>(0));
    flock.push_back(std::make_unique<Starling>(1));
    Boid* a = flock[0].get();
    Boid* b = flock[1].get();

    const float mid = (K_R_SEP + K_R_VIEW) * 0.5f;
    a->set_pos(Vec3{0.0f, 0.0f, 0.0f});
    b->set_pos(Vec3{mid, 0.0f, 0.0f});
    a->set_vel(Vec3{0.0f, 0.0f, 0.0f});
    b->set_vel(Vec3{K_MAX_SPEED, 0.0f, 0.0f});

    const Vec3 pos_before = a->get_pos();
    step(flock, 0.05f);

    CHECK(a->get_vel() == Vec3{0.0f, 0.0f, 0.0f}); 
    CHECK(a->get_pos() == pos_before);
}

TEST_CASE("an isolated boid moves in a straight line at constant speed") {
    std::vector<std::unique_ptr<Boid>> flock;
    flock.push_back(std::make_unique<Swallow>(0));
    Boid* a = flock[0].get();

    a->set_pos(Vec3{0.0f, 0.0f, 0.0f});
    a->set_vel(Vec3{K_MAX_SPEED, 0.0f, 0.0f});

    step(flock, 0.1f);

    CHECK(norm(a->get_vel()) == doctest::Approx(K_MAX_SPEED)); 
    CHECK(a->get_vel().x == doctest::Approx(K_MAX_SPEED));
    CHECK(a->get_pos().x == doctest::Approx(K_MAX_SPEED * 0.1f));
}

TEST_CASE("the filtered heading converges toward the velocity direction") {
    std::vector<std::unique_ptr<Boid>> flock;
    flock.push_back(std::make_unique<Swallow>(0));
    Boid* a = flock[0].get();

    a->set_pos(Vec3{0.0f, 0.0f, 0.0f});
    a->set_vel(Vec3{K_MAX_SPEED, 0.0f, 0.0f});

    for (int i = 0; i < 200; ++i)
        step(flock, 0.05f);

    Vec3 h = a->get_heading();
    // Check that the heading is a valid normalized vector (length of 1)
    CHECK(norm(h) == doctest::Approx(1.0f).epsilon(0.01));
    // Check that the boid is now pointing exactly in the direction of travel (+x axis)
    CHECK(h.x == doctest::Approx(1.0f).epsilon(0.01)); 
}

TEST_CASE("Wall::get_closest_point projects onto the wall plane") {
    Wall wall(Vec3{BOX_HALF_X, 0.0f, 0.0f}, Vec3{-1.0f, 0.0f, 0.0f}, WALL_THREAT_FACTOR);

    SUBCASE("orthogonal projection of an interior point") {
        Vec3 cp = wall.get_closest_point(Vec3{50.0f, 17.0f, -8.0f});
        CHECK(cp.x == doctest::Approx(BOX_HALF_X)); // x = BOX_HALF_X
        CHECK(cp.y == doctest::Approx(17.0f));      // The Y and Z coordinates must remain unchanged 
        CHECK(cp.z == doctest::Approx(-8.0f));
    }

    SUBCASE("a point already on the plane stays unchanged") {
        Vec3 on{BOX_HALF_X, 5.0f, 5.0f};
        Vec3 cp = wall.get_closest_point(on);
        CHECK(cp.x == doctest::Approx(on.x));
        CHECK(cp.y == doctest::Approx(on.y));
        CHECK(cp.z == doctest::Approx(on.z));
    }

    SUBCASE("base_threat exposed by the base class") {
        CHECK(wall.get_base_threat() == doctest::Approx(WALL_THREAT_FACTOR));
    }
}
