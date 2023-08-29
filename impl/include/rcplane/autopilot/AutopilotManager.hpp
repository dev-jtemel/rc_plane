#ifndef RCPLANE_AUTOPILOT_AUTOPILOT_MANAGER_HPP
#define RCPLANE_AUTOPILOT_AUTOPILOT_MANAGER_HPP

#include <boost/noncopyable.hpp>

#include "rcplane/autopilot/IAutopilot.hpp"
#include "rcplane/autopilot/ManualAutopilot.hpp"
#include "rcplane/common/Packet.hpp"
#include "rcplane/io/ConfigManager.hpp"

namespace rcplane {
namespace autopilot {

/**
 * @brief Maintain the different autopilots and trigger the appropriate autopilot
 * based on the received packet.
 */
class AutopilotManager : public ::boost::noncopyable {
public:
  AutopilotManager();
  ~AutopilotManager();

  /**
   * @brief Trigger the appropriate autopilot based on the received packet.
   * @param rcRxPacket The received packet.
   * @return The control surface packet to send to the MCU.
   */
  common::ControlSurfacePacket trigger(const common::RcRxPacket &rcRxPacket);

private:
  ManualAutopilot m_manualAutopilot;
  IAutopilot *m_autopilot;
};

}  // namespace autopilot
}  // namespace rcplane

#endif  //RCPLANE_AUTOPILOT_AUTOPILOT_MANAGER_HPP