# COMMON IO

add_executable(rcplane_common_tests
  main.cpp
  packet_test.cpp
  network__http_controller_test.cpp
)

target_link_libraries(rcplane_common_tests
  PUBLIC
    rcplane_common_network
    gtest
    pthread
    curl
)
