project(KuhnPokerMCCFR)
cmake_minimum_required(VERSION 3.5)

add_compile_options(-D_FORTIFY_SOURCE=2 -D_GLBICXX_ASSERTIONS
    -fasynchronous-unwind-tables -fexceptions -fpie -fpic -fstack-protector -g
    -grecord-gcc-switches -O2 -pipe -Wall -Wextra -W -Wconversion
    -Wcast-align -Wunused -Wshadow -Wold-style-cast -Wpointer-arith -Wpedantic -std=c++17)

add_link_options(-pthread)
add_definitions("-fPIC")

include_directories(src)
file(GLOB SOURCES src/*.h src/*.cpp)
add_executable(4th_compute_dist src/4th_round/main.cpp)
add_executable(3rd_compute_dist src/3rd_round/main.cpp)
target_compile_features(4th_compute_dist PRIVATE cxx_lambda_init_captures)
target_compile_features(3rd_compute_dist PRIVATE cxx_lambda_init_captures)
