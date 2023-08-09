# COMMON IO

add_library(rcplane_common_io
  src/io/journal_impl.cpp
  src/io/serial_controller.cpp
)

target_link_libraries(rcplane_common_io
  PRIVATE
    boost_log
    boost_log_setup
    boost_thread
)

add_library(rcplane_common_network
  src/network/http_controller.cpp
)

target_link_libraries(rcplane_common_network
  PUBLIC
    rcplane_common_io
    curl
)

if (BUILD_TESTS)
  add_subdirectory(test)
endif()
