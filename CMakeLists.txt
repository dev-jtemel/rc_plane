cmake_minimum_required(VERSION 3.16)
project(rc-plane)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3 -g -Wall")

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
find_package(Threads REQUIRED)
find_package(Boost 1.58.0 REQUIRED)

include_directories(
  impl/include
  third_party
)

if (BUILD_TESTS)
  MESSAGE("Building with tests")
  enable_testing()
  SET(CMAKE_BUILD_TYPE Debug)
  SET(CMAKE_CXX_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage")
  SET(CMAKE_C_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage")
  add_compile_definitions(VIRTUAL_TEST=virtual)
  add_subdirectory(impl/test)
else()
  add_compile_definitions(VIRTUAL_TEST=)
endif()

add_subdirectory(impl)