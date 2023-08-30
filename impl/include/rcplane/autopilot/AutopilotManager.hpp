#ifndef RCPLANE_AUTOPILOT_AUTOPILOT_MANAGER_HPP
#define RCPLANE_AUTOPILOT_AUTOPILOT_MANAGER_HPP

#include <boost/noncopyable.hpp>

#include "rcplane/autopilot/IAutopilot.hpp"
#include "rcplane/autopilot/ManualAutopilot.hpp"
#include "rcplane/autopilot/StabilizeAutopilot.hpp"
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
   * @return bool True if the active autopilot is a manual autopilot.
   */
  bool isInManualMode() const;

  /**
   * @return bool True if the active autopilot is a stabilized autopilot.
   */
  bool isInStabilizeMode() const;

  /**
   * @brief Trigger the appropriate autopilot based on the received packet.
   * @param rcRxPacket The received packet.
   * @return The control surface packet to send to the MCU.
   */
  common::ControlSurfacePacket trigger(const common::RcRxPacket &rcRxPacket,
                                       const common::ImuPacket &imuPacket);

private:
  /**
   * @brief Change the underlying autopilot, if necessary.
   * @param state The state reported by the RC RX.
   */
  void handleState(const uint8_t state);

  StabilizeAutopilot m_stabilizeAutopilot;
  ManualAutopilot m_manualAutopilot;
  IAutopilot *m_autopilot;

  uint8_t m_prevState{};
};

}  // namespace autopilot
}  // namespace rcplane

#endif  //RCPLANE_AUTOPILOT_AUTOPILOT_MANAGER_HPP