#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <memory>
#include <vector>

#include "boid.hpp"
#include "configs.hpp"
#include "danger.hpp"
#include "starling.hpp"
#include "swallow.hpp"
#include "vec3.hpp"
#include "wall.hpp"

// I parametri di Swallow/Starling/Seagull sono identici:
//   max_speed = 60, r_view = 60, r_sep = 30.
// Le distanze nei test sono scelte rispetto a queste soglie.
static constexpr float kMaxSpeed = 60.0f;
static constexpr float kRView = 60.0f;
static constexpr float kRSep = 30.0f;

// Niente pericoli: array di sei puntatori nulli.
static Danger* no_dangers[6] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

// Esegue un passo della simulazione come fa l'Engine:
// prima tutti leggono lo stato vecchio (evolve), poi tutti lo aggiornano (apply).
static void step(std::vector<std::unique_ptr<Boid>>& flock, float dt) {
    for (auto& b : flock) b->evolve(flock, no_dangers, dt);
    for (auto& b : flock) b->apply(dt);
}

TEST_CASE("set/get di posizione e velocita'") {
    Swallow b(0);
    b.set_pos(Vec3{10.0f, -20.0f, 30.0f});
    CHECK(b.get_pos() == Vec3{10.0f, -20.0f, 30.0f});
    b.set_vel(Vec3{1.0f, 2.0f, 3.0f});
    CHECK(b.get_vel() == Vec3{1.0f, 2.0f, 3.0f});
}

TEST_CASE("clamp della velocita' al max_speed") {
    Swallow b(0);

    SUBCASE("velocita' eccessiva viene troncata a max_speed") {
        b.set_vel(Vec3{1000.0f, 0.0f, 0.0f});
        CHECK(norm(b.get_vel()) == doctest::Approx(kMaxSpeed));
        CHECK(b.get_vel().x == doctest::Approx(kMaxSpeed));   // direzione preservata
    }

    SUBCASE("velocita' entro il limite resta invariata") {
        Vec3 v{3.0f, 4.0f, 0.0f};   // norma 5 < 60
        b.set_vel(v);
        CHECK(b.get_vel() == v);
    }
}

TEST_CASE("wrap toroidale della posizione ai bordi del box rettangolare") {
    const float X = Config::BOX_HALF_X;
    const float Y = Config::BOX_HALF_Y;
    Swallow b(0);

    SUBCASE("oltre il bordo positivo in x rientra dal lato opposto") {
        b.set_pos(Vec3{X + 50.0f, 0.0f, 0.0f});
        CHECK(b.get_pos().x == doctest::Approx(-X + 50.0f));
    }

    SUBCASE("oltre il bordo negativo in y rientra dal lato opposto") {
        b.set_pos(Vec3{0.0f, -Y - 30.0f, 0.0f});
        CHECK(b.get_pos().y == doctest::Approx(Y - 30.0f));
    }

    SUBCASE("dentro il box resta invariata") {
        Vec3 p{10.0f, -10.0f, 5.0f};
        b.set_pos(p);
        CHECK(b.get_pos() == p);
    }
}

TEST_CASE("separazione: due boid troppo vicini si allontanano") {
    std::vector<std::unique_ptr<Boid>> flock;
    flock.push_back(std::make_unique<Swallow>(0));
    flock.push_back(std::make_unique<Swallow>(1));
    Boid* a = flock[0].get();
    Boid* b = flock[1].get();

    const float close = kRSep * 0.3f;   // ben dentro il raggio di separazione
    a->set_pos(Vec3{0.0f, 0.0f, 0.0f});
    b->set_pos(Vec3{close, 0.0f, 0.0f});
    a->set_vel(Vec3{0.0f, 0.0f, 0.0f});
    b->set_vel(Vec3{0.0f, 0.0f, 0.0f});

    const float before = dist(a->get_pos(), b->get_pos());
    step(flock, 0.1f);
    const float after = dist(a->get_pos(), b->get_pos());

    CHECK(after > before);              // la distanza aumenta
    CHECK(a->get_pos().x < 0.0f);       // a fugge verso -x
    CHECK(b->get_pos().x > 0.0f);       // b fugge verso +x
}

TEST_CASE("coesione: un boid sterza verso un vicino fermo della stessa razza") {
    std::vector<std::unique_ptr<Boid>> flock;
    flock.push_back(std::make_unique<Swallow>(0));
    flock.push_back(std::make_unique<Swallow>(1));
    Boid* a = flock[0].get();
    Boid* b = flock[1].get();

    // Oltre r_sep ma dentro r_view: niente separazione, solo coesione (vicino fermo).
    const float mid = (kRSep + kRView) * 0.5f;
    a->set_pos(Vec3{0.0f, 0.0f, 0.0f});
    b->set_pos(Vec3{mid, 0.0f, 0.0f});
    a->set_vel(Vec3{0.0f, 0.0f, 0.0f});
    b->set_vel(Vec3{0.0f, 0.0f, 0.0f});

    step(flock, 0.05f);

    CHECK(a->get_vel().x > 0.0f);   // accelera verso il vicino
}

TEST_CASE("allineamento: un boid adegua la propria direzione a quella del vicino") {
    const float saved_cohes = Config::g_params.w_cohes;
    Config::g_params.w_cohes = 0.0f;   // isola l'allineamento dalla coesione

    std::vector<std::unique_ptr<Boid>> flock;
    flock.push_back(std::make_unique<Swallow>(0));
    flock.push_back(std::make_unique<Swallow>(1));
    Boid* a = flock[0].get();
    Boid* b = flock[1].get();

    const float mid = (kRSep + kRView) * 0.5f;
    a->set_pos(Vec3{0.0f, 0.0f, 0.0f});
    b->set_pos(Vec3{0.0f, mid, 0.0f});         // sfalsato in y per non influenzare x via coesione
    a->set_vel(Vec3{0.0f, 0.0f, 0.0f});
    b->set_vel(Vec3{kMaxSpeed, 0.0f, 0.0f});   // il vicino vola verso +x

    step(flock, 0.05f);

    CHECK(a->get_vel().x > 0.0f);   // a si allinea verso +x

    Config::g_params.w_cohes = saved_cohes;
}

TEST_CASE("separazione tra razze diverse: avviene comunque") {
    std::vector<std::unique_ptr<Boid>> flock;
    flock.push_back(std::make_unique<Swallow>(0));
    flock.push_back(std::make_unique<Starling>(1));
    Boid* a = flock[0].get();
    Boid* b = flock[1].get();

    const float close = kRSep * 0.3f;
    a->set_pos(Vec3{0.0f, 0.0f, 0.0f});
    b->set_pos(Vec3{close, 0.0f, 0.0f});
    a->set_vel(Vec3{0.0f, 0.0f, 0.0f});
    b->set_vel(Vec3{0.0f, 0.0f, 0.0f});

    step(flock, 0.1f);

    CHECK(a->get_pos().x < 0.0f);   // la separazione ignora la razza
    CHECK(b->get_pos().x > 0.0f);
}

TEST_CASE("nessun allineamento tra razze diverse") {
    std::vector<std::unique_ptr<Boid>> flock;
    flock.push_back(std::make_unique<Swallow>(0));
    flock.push_back(std::make_unique<Starling>(1));
    Boid* a = flock[0].get();
    Boid* b = flock[1].get();

    // Dentro r_view ma oltre r_sep: con la stessa razza ci sarebbe allineamento/coesione,
    // con razze diverse non deve succedere nulla.
    const float mid = (kRSep + kRView) * 0.5f;
    a->set_pos(Vec3{0.0f, 0.0f, 0.0f});
    b->set_pos(Vec3{mid, 0.0f, 0.0f});
    a->set_vel(Vec3{0.0f, 0.0f, 0.0f});
    b->set_vel(Vec3{kMaxSpeed, 0.0f, 0.0f});

    const Vec3 pos_before = a->get_pos();
    step(flock, 0.05f);

    CHECK(a->get_vel() == Vec3{0.0f, 0.0f, 0.0f});   // resta fermo
    CHECK(a->get_pos() == pos_before);
}

TEST_CASE("un boid isolato si muove in linea retta a velocita' costante") {
    std::vector<std::unique_ptr<Boid>> flock;
    flock.push_back(std::make_unique<Swallow>(0));
    Boid* a = flock[0].get();

    a->set_pos(Vec3{0.0f, 0.0f, 0.0f});
    a->set_vel(Vec3{kMaxSpeed, 0.0f, 0.0f});

    step(flock, 0.1f);

    CHECK(norm(a->get_vel()) == doctest::Approx(kMaxSpeed));   // nessuna forza: modulo invariato
    CHECK(a->get_vel().x == doctest::Approx(kMaxSpeed));
    CHECK(a->get_pos().x == doctest::Approx(kMaxSpeed * 0.1f));
}

TEST_CASE("l'heading filtrato converge verso la direzione della velocita'") {
    std::vector<std::unique_ptr<Boid>> flock;
    flock.push_back(std::make_unique<Swallow>(0));
    Boid* a = flock[0].get();

    a->set_pos(Vec3{0.0f, 0.0f, 0.0f});
    a->set_vel(Vec3{kMaxSpeed, 0.0f, 0.0f});

    for (int i = 0; i < 200; ++i) step(flock, 0.05f);

    Vec3 h = a->get_heading();
    CHECK(norm(h) == doctest::Approx(1.0f).epsilon(0.01));
    CHECK(h.x == doctest::Approx(1.0f).epsilon(0.01));   // punta verso +x
}

TEST_CASE("Wall::get_closest_point proietta sul piano della parete") {
    Wall wall(Vec3{Config::BOX_HALF_X, 0.0f, 0.0f}, Vec3{-1.0f, 0.0f, 0.0f},
              Config::WALL_THREAT_FACTOR);

    SUBCASE("proiezione ortogonale di un punto interno") {
        Vec3 cp = wall.get_closest_point(Vec3{50.0f, 17.0f, -8.0f});
        CHECK(cp.x == doctest::Approx(Config::BOX_HALF_X));   // sul piano x = BOX_HALF_X
        CHECK(cp.y == doctest::Approx(17.0f));               // componenti tangenti invariate
        CHECK(cp.z == doctest::Approx(-8.0f));
    }

    SUBCASE("un punto gia' sul piano resta invariato") {
        Vec3 on{Config::BOX_HALF_X, 5.0f, 5.0f};
        Vec3 cp = wall.get_closest_point(on);
        CHECK(cp.x == doctest::Approx(on.x));
        CHECK(cp.y == doctest::Approx(on.y));
        CHECK(cp.z == doctest::Approx(on.z));
    }

    SUBCASE("base_threat esposto dalla classe base") {
        CHECK(wall.get_base_threat() == doctest::Approx(Config::WALL_THREAT_FACTOR));
    }
}
