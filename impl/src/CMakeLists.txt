add_library(rcplane_io
  io/ConfigManager.cpp
  io/SerialController.cpp
)
target_link_libraries(rcplane_io
  PRIVATE
    boost_log
    boost_log_setup
    boost_system
    boost_thread
    pthread
)

add_executable(SomController
  som/SomController.cpp
)

target_link_libraries(SomController
  PUBLIC
    rcplane_io
)