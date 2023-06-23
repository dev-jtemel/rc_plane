# COMMON IO

add_executable(rcplane_common_tests
  main.cpp
  packet_test.cpp
)

target_link_libraries(rcplane_common_tests
  PUBLIC
    gtest
)
