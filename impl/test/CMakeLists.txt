include_directories(mock)

add_executable(RcplaneTests
  main.cpp
  unit/ConfigManagerTest.cpp
  unit/SerialControllerTest.cpp
)

target_link_libraries(RcplaneTests
  PUBLIC
    rcplane_io_new
  PRIVATE
    gmock
    gtest
)