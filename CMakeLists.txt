cmake_minimum_required(VERSION 3.16)
project(rc-plane)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -g -Wall")

find_package(Threads REQUIRED)

include_directories(
  common/include
  som/include
  third_party
)

enable_testing()
SET( CMAKE_BUILD_TYPE Debug )
SET(CMAKE_CXX_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage")

add_subdirectory(common)
add_subdirectory(som)
