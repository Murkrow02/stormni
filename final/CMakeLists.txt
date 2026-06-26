cmake_minimum_required(VERSION 3.16)
project(storni CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(raylib REQUIRED)

# Tutti i target trovano gli header del progetto (vec3.hpp, doctest.h, ...).
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Warning sempre attivi.
add_compile_options(-Wall -Wextra -Wpedantic)

# Sanitizer e asserzioni della standard library in Debug (vedi consegna).
# Su macOS (Darwin recenti) il runtime di AddressSanitizer va in hang all'avvio,
# quindi i sanitizer si attivano solo su piattaforme non-Apple (es. l'ambiente
# Linux di valutazione). I flag non sono altrimenti rilassati.
if(NOT APPLE)
    add_compile_options(
            $<$<CONFIG:Debug>:-fsanitize=address,undefined>
            $<$<CONFIG:Debug>:-fno-omit-frame-pointer>)
    add_link_options($<$<CONFIG:Debug>:-fsanitize=address,undefined>)
endif()
add_compile_definitions($<$<CONFIG:Debug>:_GLIBCXX_ASSERTIONS>)

# ---- Applicazione grafica ----
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
        swallow.hpp
        seagull.hpp
        seagull.cpp
        raygui.h
        raygui_impl.cpp)
target_link_libraries(game PRIVATE raylib)

# ---- Test ----
enable_testing()

# Matematica vettoriale: header-only, nessuna dipendenza esterna.
add_executable(test_vec3 test_vec3.cpp vec3.hpp)
add_test(NAME test_vec3 COMMAND test_vec3)

# Logica dei boid (regole di Reynolds, wrap, clamp) e pareti.
# Dipende da raylib solo per il tipo Color.
add_executable(test_boid test_boid.cpp
        boid.cpp
        swallow.cpp
        starling.cpp)
target_link_libraries(test_boid PRIVATE raylib)
add_test(NAME test_boid COMMAND test_boid)
