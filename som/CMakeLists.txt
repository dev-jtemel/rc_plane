
#############
# Simulator

add_executable(simulator
  src/simulator.cpp
)

target_link_libraries(simulator PUBLIC
  rcplane_common_io
)
