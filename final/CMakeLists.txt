cmake_minimum_required(VERSION 3.10)
project(game)

find_package(raylib REQUIRED)

add_executable(game main.cpp
        vec3.hpp
        configs.hpp
        boid.cpp
        boid.hpp
        starling.cpp
        starling.hpp
        danger.hpp
        wall.hpp
        utils.cpp
        engine.cpp
        engine.hpp
        swallow.cpp
        swallow.hpp)
target_link_libraries(game PRIVATE raylib)