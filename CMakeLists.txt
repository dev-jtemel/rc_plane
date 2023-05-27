cmake_minimum_required(VERSION 3.16)
project(rc-plane)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-O3")

find_package(Threads REQUIRED)

include_directories(common/include)
add_subdirectory(common)

if(PC_BUILD)
  include_directories(pc/include)
  add_subdirectory(pc)
elseif(SOM_BUILD)
  include_directories(som/include)
  add_subdirectory(som)
else()
  message(FATAL_ERROR "Please specifiy build type: PC_BUILD or SOM_BUILD")
endif()
