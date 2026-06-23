cmake_minimum_required(VERSION 3.10)
project(game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)

add_executable(game main.cpp
        vec3.hpp
        configs.hpp
        sim_params.hpp
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
        swallow.hpp
        raygui.h
        raygui_impl.cpp)
target_include_directories(game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(game PRIVATE raylib)