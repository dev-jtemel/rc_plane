add_library(som-position
  src/position/gps_controller.cpp
)

target_link_libraries(som-position
  rcplane_common_io
)

#############
# RCPLANE SoM Controller

add_executable(som-controller
  src/controller.cpp
)

target_link_libraries(som-controller PUBLIC
  rcplane_common_io
  rcplane_common_network
  som-position
  pthread
)
