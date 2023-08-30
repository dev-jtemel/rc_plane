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

add_library(rcplane_autopilot
  autopilot/AutopilotManager.cpp
  autopilot/AutopilotUtility.cpp
  autopilot/ManualAutopilot.cpp
  autopilot/StabilizeAutopilot.cpp
)
target_link_libraries(rcplane_autopilot
  PUBLIC
    rcplane_io
)

add_library(rcplane_som
  som/SomController.cpp
)

target_link_libraries(rcplane_som
  PUBLIC
    rcplane_io
    rcplane_autopilot
)

add_executable(SomController
  som/main.cpp
)

target_link_libraries(SomController
  PUBLIC
    rcplane_io
    rcplane_autopilot
    rcplane_som
)