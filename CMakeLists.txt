project(IRKEKOAbstraction)
cmake_minimum_required(VERSION 3.5)

add_compile_options(-D_FORTIFY_SOURCE=2 -D_GLBICXX_ASSERTIONS
    -fasynchronous-unwind-tables -fexceptions -fpie -fpic -fstack-protector -g
    -grecord-gcc-switches -O2 -pipe -std=c++17)

add_definitions("-fPIC")

enable_testing()
add_subdirectory(extern/googletest)

add_library(4th_compute_dist_lib STATIC src/4th_round/fourthround.cpp)

add_executable(fourth_round_tests tests/FourthRoundTests.cpp)
target_link_libraries(fourth_round_tests PUBLIC
    gtest_main
    4th_compute_dist_lib
)

add_test(
    NAME fourth_round_tests
    COMMAND fourth_round_tests
)

include_directories(src)
file(GLOB SOURCES src/*.h src/*.cpp)
add_executable(4th_compute_dist src/4th_round/main.cpp src/4th_round/fourthround.cpp)
add_executable(3rd_compute_dist src/3rd_round/main.cpp)
add_executable(2nd_compute_dist src/2nd_round/main.cpp)
add_executable(1st_compute_dist src/2nd_round/main.cpp)
target_compile_features(4th_compute_dist PRIVATE cxx_lambda_init_captures)
target_compile_features(3rd_compute_dist PRIVATE cxx_lambda_init_captures)
target_compile_features(2nd_compute_dist PRIVATE cxx_lambda_init_captures)
target_compile_features(1st_compute_dist PRIVATE cxx_lambda_init_captures)
