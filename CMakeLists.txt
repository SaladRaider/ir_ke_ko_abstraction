project(IRKEKOAbstraction)
cmake_minimum_required(VERSION 3.5)

add_compile_options(-D_FORTIFY_SOURCE=2 -D_GLBICXX_ASSERTIONS -O3 -pipe -std=c++17)

enable_testing()
add_subdirectory(extern/googletest)

add_library(4th_compute_dist_lib STATIC src/4th_round/fourthround.cpp)
add_library(3rd_compute_dist_lib STATIC src/3rd_round/thirdround.cpp)
add_library(2nd_compute_dist_lib STATIC src/2nd_round/secondround.cpp)

add_executable(fourth_round_tests tests/FourthRoundTests.cpp)
add_executable(third_round_tests tests/ThirdRoundTests.cpp)
add_executable(second_round_tests tests/SecondRoundTests.cpp)
add_executable(integration_tests tests/IntegrationTests.cpp)
target_link_libraries(fourth_round_tests PUBLIC
    gtest_main
    4th_compute_dist_lib
)
target_link_libraries(third_round_tests PUBLIC
    gtest_main
    3rd_compute_dist_lib
)
target_link_libraries(second_round_tests PUBLIC
    gtest_main
    2nd_compute_dist_lib
)
target_link_libraries(integration_tests PUBLIC
    gtest_main
    2nd_compute_dist_lib
    3rd_compute_dist_lib
    4th_compute_dist_lib
)

add_test(
    NAME fourth_round_tests
    COMMAND fourth_round_tests
)
add_test(
    NAME third_round_tests
    COMMAND third_round_tests
)
add_test(
    NAME second_round_tests
    COMMAND second_round_tests
)

include_directories(src)
file(GLOB SOURCES src/*.h src/*.cpp)
add_executable(4th_compute_dist src/4th_round/main.cpp src/4th_round/fourthround.cpp)
add_executable(3rd_compute_dist src/3rd_round/main.cpp src/3rd_round/thirdround.cpp)
add_executable(2nd_compute_dist src/2nd_round/main.cpp src/2nd_round/secondround.cpp)
add_executable(1st_compute_dist src/1st_round/main.cpp)
target_compile_features(4th_compute_dist PRIVATE cxx_lambda_init_captures)
target_compile_features(3rd_compute_dist PRIVATE cxx_lambda_init_captures)
target_compile_features(2nd_compute_dist PRIVATE cxx_lambda_init_captures)
target_compile_features(1st_compute_dist PRIVATE cxx_lambda_init_captures)
