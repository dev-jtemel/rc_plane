#include "rcplane/autopilot/ManualAutopilot.hpp"
#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace autopilot {

ManualAutopilot::ManualAutopilot() : IAutopilot() { RCPLANE_LOG_METHOD(); }

ManualAutopilot::~ManualAutopilot() { RCPLANE_LOG_METHOD(); }

void ManualAutopilot::trigger(
    common::ControlSurfacePacket &controlSurfacePacket,
    const common::RcRxPacket &rcRxPacket) {
  RCPLANE_LOG_METHOD();

  controlSurfacePacket.motorSpeed =
      bindThrottleToRange(rcRxPacket.motorStickPosition);
  controlSurfacePacket.aileronDeflection =
      bindAileronDeflectionToRange(rcRxPacket.aileronStickPosition);
  controlSurfacePacket.elevatorDeflection =
      bindElevatorDeflectionToRange(rcRxPacket.elevatorStickPosition);
  controlSurfacePacket.rudderDeflection =
      bindRudderDeflectionToRange(rcRxPacket.rudderStickPosition);
}

}  // namespace autopilot
}  // namespace rcplane