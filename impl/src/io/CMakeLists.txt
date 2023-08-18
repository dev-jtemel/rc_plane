add_library(rcplane_io
  config_manager.cpp
  serial_controller.cpp
)

target_link_libraries(rcplane_io
  PUBLIC
    boost_log
    boost_log_setup
    boost_system
    boost_thread
    pthread
)
