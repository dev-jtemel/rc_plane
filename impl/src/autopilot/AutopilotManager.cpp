#include "rcplane/autopilot/AutopilotManager.hpp"

#include "rcplane/common/State.hpp"
#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace autopilot {

AutopilotManager::AutopilotManager()
  : m_manualAutopilot(), m_autopilot(&m_manualAutopilot) {
  RCPLANE_LOG_METHOD();
}

AutopilotManager::~AutopilotManager() { RCPLANE_LOG_METHOD(); }

bool AutopilotManager::isInManualMode() const {
  RCPLANE_LOG_METHOD();
  return m_autopilot == &m_manualAutopilot;
}

bool AutopilotManager::isInStabilizeMode() const {
  RCPLANE_LOG_METHOD();
  return m_autopilot == &m_stabilizeAutopilot;
}

common::ControlSurfacePacket AutopilotManager::trigger(
    const common::RcRxPacket &rcRxPacket,
    const common::ImuPacket &imuPacket) {
  RCPLANE_LOG_METHOD();

  handleState(rcRxPacket.state & common::state::kAUTOPILOT_MASK);

  common::ControlSurfacePacket controlSurfacePacket;
  m_autopilot->trigger(controlSurfacePacket, rcRxPacket, imuPacket);
  return controlSurfacePacket;
}

void AutopilotManager::handleState(const uint8_t state) {
  if (state == m_prevState) { return; }

  if (state & common::state::kASSISTANCE_FLAG) {
    m_autopilot = &m_stabilizeAutopilot;
    RCPLANE_LOG(info, "Switched to Stabilize Autopilot.");
  } else {
    m_autopilot = &m_manualAutopilot;
    RCPLANE_LOG(info, "Switched to Manual Autopilot.");
  }

  m_prevState = state;
}

}  // namespace autopilot
}  // namespace rcplane