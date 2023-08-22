include_directories(mock)

add_executable(rcplane_unit_test
  main.cpp
  unit/packet_test.cpp
  unit/config_manager_test.cpp
  unit/serial_controller_test.cpp
)

target_link_libraries(rcplane_unit_test
  PUBLIC
    rcplane_io
    gtest
    gmock
)