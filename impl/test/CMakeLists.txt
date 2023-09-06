include_directories(.)

add_executable(RcplaneTests
  main.cpp
  unit/ConfigManagerTest.cpp
  unit/SerialControllerTest.cpp
  unit/ManualAutopilotTest.cpp
  unit/StabilizeAutopilotTest.cpp
  unit/AutopilotManagerTest.cpp
  unit/TelemetryMQTest.cpp

  integration/SomControllerTest.cpp
)

target_link_libraries(RcplaneTests
  PUBLIC
    rcplane_io
    rcplane_io_telemetry_tx
    rcplane_io_telemetry_rx
    rcplane_autopilot
    rcplane_som
  PRIVATE
    gmock
    gtest
)