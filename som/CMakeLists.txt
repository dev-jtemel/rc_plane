
#############
# RCPLANE SoM Controller

add_executable(som-controller
  src/controller.cpp
)

target_link_libraries(som-controller PUBLIC
  rcplane_common_io
  rcplane_common_network
  pthread
)
