# COMMON IO

add_library(rcplane_common_io
  src/io/journal_impl.cpp
  src/io/serial_controller.cpp
)

add_library(rcplane_common_network
  src/network/http_controller.cpp
)

target_link_libraries(rcplane_common_network
  rcplane_common_io
)

add_subdirectory(test)
