cmake_minimum_required(VERSION 3.16)
project(rc-plane)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")

find_package(Threads REQUIRED)

include_directories(
  common/include
  som/include
  third_party
)

add_subdirectory(common)
add_subdirectory(som)
