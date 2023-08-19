add_library(rcplane_io
  io/config_manager.cpp
  io/serial_controller.cpp
)

target_link_libraries(rcplane_io
  PRIVATE
    boost_log
    boost_log_setup
    boost_system
    boost_thread
    pthread
)

add_library(rcplane_hw
  hw/control_surface_manager.cpp
  hw/imu_manager.cpp
)

target_link_libraries(rcplane_hw
  PUBLIC
    rcplane_io
)

add_executable(som_controller
  som/main.cpp
)

target_link_libraries(som_controller
  PUBLIC
    rcplane_io
    rcplane_hw
  PRIVATE
    boost_log
    boost_log_setup
    boost_system
    boost_thread
    pthread
)