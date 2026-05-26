cmake_minimum_required(VERSION 3.10)
project(game)

find_package(raylib REQUIRED)

add_executable(game main.cpp
        vec3.h
        configs.h
        boid.cpp
        boid.h)
target_link_libraries(game PRIVATE raylib)