cmake_minimum_required(VERSION 3.16)
project(storni CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(raylib REQUIRED)

# Header del progetto (src/) e librerie esterne header-only (external/).
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/external)

# Warning sempre attivi.
add_compile_options(-Wall -Wextra -Wpedantic)

# ---- Applicazione grafica ----
add_executable(game
        src/main.cpp
        src/vec3.hpp
        src/configs.hpp
        src/boid.cpp
        src/boid.hpp
        src/starling.cpp
        src/starling.hpp
        src/danger.hpp
        src/wall.hpp
        src/utils.cpp
        src/engine.cpp
        src/engine.hpp
        src/swallow.cpp
        src/swallow.hpp
        src/seagull.hpp
        src/seagull.cpp
        external/raygui.h
        src/raygui_impl.cpp)
target_link_libraries(game PRIVATE raylib)

# ---- Test ----
enable_testing()

# Matematica vettoriale: header-only, nessuna dipendenza esterna.
add_executable(test_vec3 tests/test_vec3.cpp src/vec3.hpp)
add_test(NAME test_vec3 COMMAND test_vec3)

# Logica dei boid (regole di Reynolds, wrap, clamp) e pareti.
# Dipende da raylib solo per il tipo Color.
add_executable(test_boid
        tests/test_boid.cpp
        src/boid.cpp
        src/swallow.cpp
        src/starling.cpp)
target_link_libraries(test_boid PRIVATE raylib)
add_test(NAME test_boid COMMAND test_boid)
