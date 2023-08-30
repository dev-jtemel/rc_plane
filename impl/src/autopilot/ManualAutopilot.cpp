#include "rcplane/autopilot/ManualAutopilot.hpp"
#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace autopilot {

ManualAutopilot::ManualAutopilot(const AutopilotUtility &autopilotUtility)
  : IAutopilot(), m_autopilotUtility(autopilotUtility) {
  RCPLANE_LOG_METHOD();
}

ManualAutopilot::~ManualAutopilot() { RCPLANE_LOG_METHOD(); }

void ManualAutopilot::trigger(
    common::ControlSurfacePacket &controlSurfacePacket,
    const common::RcRxPacket &rcRxPacket,
    const common::ImuPacket &) {
  RCPLANE_LOG_METHOD();

  controlSurfacePacket.motorSpeed =
      m_autopilotUtility.bindRcThrottle(rcRxPacket.motorStickPosition);
  controlSurfacePacket.aileronDeflection =
      m_autopilotUtility.bindRcAileronDeflection(
          rcRxPacket.aileronStickPosition);
  controlSurfacePacket.elevatorDeflection =
      m_autopilotUtility.bindRcElevatorDeflection(
          rcRxPacket.elevatorStickPosition);
  controlSurfacePacket.rudderDeflection =
      m_autopilotUtility.bindRcRudderDeflection(rcRxPacket.rudderStickPosition);
}

}  // namespace autopilot
}  // namespace rcplane