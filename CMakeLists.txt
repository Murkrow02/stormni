cmake_minimum_required(VERSION 3.16)
project(storni LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Homebrew install path (Apple Silicon).
list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew" "/usr/local")

# raylib installata via `brew install raylib`.
# Fornisce il target imported `raylib` con include dirs + librerie + flags.
find_package(raylib REQUIRED CONFIG)

# Una funzione per registrare un target per ogni stage.
# Ogni stage = una sottocartella con il proprio main.cpp.
function(add_stage stage_dir)
    set(target ${stage_dir})
    add_executable(${target} ${CMAKE_CURRENT_SOURCE_DIR}/${stage_dir}/main.cpp)
    target_link_libraries(${target} PRIVATE raylib)
    target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/${stage_dir})
    # In CLion mette l'eseguibile in cmake-build-debug/<target>; lo nominiamo "murmur"
    # per coerenza con build.sh, ma il NAME diverso per target serve per non collidere.
    set_target_properties(${target} PROPERTIES OUTPUT_NAME "murmur")
    # Output di ogni stage va in build/<stage>/murmur.
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${stage_dir})
endfunction()

add_stage(00_setup)
add_stage(01_punto)
add_stage(02_movimento)
add_stage(03_vettori)
add_stage(04_stormo)
add_stage(05_bordi)
add_stage(06_separazione)
add_stage(07_allineamento)
add_stage(08_coesione)
add_stage(09_parametri)
add_stage(10_predatore_e_ostacoli)
add_stage(11_3d_bonus)
