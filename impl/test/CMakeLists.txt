include_directories(.)

add_executable(RcplaneTests
  main.cpp
  unit/ConfigManagerTest.cpp
  unit/SerialControllerTest.cpp
  unit/ManualAutopilotTest.cpp
  unit/AutopilotManagerTest.cpp
)

target_link_libraries(RcplaneTests
  PUBLIC
    rcplane_io
    rcplane_autopilot
  PRIVATE
    gmock
    gtest
)