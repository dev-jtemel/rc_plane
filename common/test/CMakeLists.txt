# COMMON IO

add_executable(rcplane_common_tests
  main.cpp
  packet_test.cpp
  journal_test.cpp
  serial_controller_test.cpp
  network__http_controller_test.cpp
)

target_link_libraries(rcplane_common_tests
  PUBLIC
    rcplane_common_io
    rcplane_common_network
    gtest
    gmock
    pthread
    curl
)
