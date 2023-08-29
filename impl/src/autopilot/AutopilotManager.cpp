#include "rcplane/autopilot/AutopilotManager.hpp"

#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace autopilot {

AutopilotManager::AutopilotManager()
  : m_manualAutopilot(), m_autopilot(&m_manualAutopilot) {
  RCPLANE_LOG_METHOD();
}

AutopilotManager::~AutopilotManager() { RCPLANE_LOG_METHOD(); }

common::ControlSurfacePacket AutopilotManager::trigger(
    const common::RcRxPacket &rcRxPacket) {
  RCPLANE_LOG_METHOD();

  common::ControlSurfacePacket controlSurfacePacket;

  m_autopilot->trigger(controlSurfacePacket, rcRxPacket);

  return controlSurfacePacket;
}

}  // namespace autopilot
}  // namespace rcplane