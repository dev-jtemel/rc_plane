include_directories(.)

add_executable(RcplaneTests
  main.cpp
  unit/ConfigManagerTest.cpp
  unit/SerialControllerTest.cpp
)

target_link_libraries(RcplaneTests
  PUBLIC
    rcplane_io
  PRIVATE
    gmock
    gtest
)