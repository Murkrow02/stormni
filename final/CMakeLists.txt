cmake_minimum_required(VERSION 3.28)
project(storni VERSION 0.1.0)

include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

string(APPEND CMAKE_CXX_FLAGS
      " -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion"
      " -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast"
      " -fno-omit-frame-pointer")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS " -D_GLIBCXX_ASSERTIONS")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  string(APPEND CMAKE_CXX_FLAGS " -D_LIBCPP_HARDENING_MODE=_LIBCPP_HARDENING_MODE_EXTENSIVE")
endif()

string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined")
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  string(APPEND CMAKE_CXX_FLAGS " -D_GLIBCXX_SANITIZE_STD_ALLOCATOR")
endif()
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined")


find_package(raylib REQUIRED)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/external)

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

if (BUILD_TESTING)
    

    add_executable(test_vec3 tests/test_vec3.cpp src/vec3.hpp)
    add_test(NAME test_vec3 COMMAND test_vec3)

    
    add_executable(test_boid
            tests/test_boid.cpp
            src/boid.cpp
            src/swallow.cpp
            src/starling.cpp)
    target_link_libraries(test_boid PRIVATE raylib)
    add_test(NAME test_boid COMMAND test_boid)

endif()