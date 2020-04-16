project(KuhnPokerMCCFR)
cmake_minimum_required(VERSION 3.5)

add_compile_options(-D_FORTIFY_SOURCE=2 -D_GLBICXX_ASSERTIONS
    -fasynchronous-unwind-tables -fexceptions -fpie -fpic -fstack-protector -g
    -grecord-gcc-switches -O2 -pipe -Wall -Wextra -W -Wconversion
    -Wcast-align -Wunused -Wshadow -Wold-style-cast -Wpointer-arith -Wpedantic -std=c++17)

include_directories(src)
file(GLOB SOURCES src/*.h src/*.cpp)
add_executable(compute_dist src/main.cpp)
target_compile_features(compute_dist PRIVATE cxx_lambda_init_captures)
