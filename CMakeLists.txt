cmake_minimum_required(VERSION 3.14)
project(Lab7_ODETask CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(ode_lib STATIC src/ODETask.cpp)

add_executable(lab7_demo src/main.cpp)
target_link_libraries(lab7_demo PRIVATE ode_lib)

add_executable(bad_exceptions src/bad_exceptions.cpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests tests/test_ode.cpp)
target_link_libraries(unit_tests PRIVATE ode_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)