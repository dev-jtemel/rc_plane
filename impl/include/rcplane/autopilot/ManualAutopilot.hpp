#ifndef RCPLANE_AUTOPILOT_MANUAL_AUTOPILOT_HPP
#define RCPLANE_AUTOPILOT_MANUAL_AUTOPILOT_HPP

#include <boost/noncopyable.hpp>

#include "rcplane/autopilot/AutopilotUtility.hpp"
#include "rcplane/autopilot/IAutopilot.hpp"

namespace rcplane {
namespace autopilot {

/**
 * @brief Dummy autopilot that does nothing. Used for full user control.
 */
class ManualAutopilot : public IAutopilot {
public:
  ManualAutopilot(const AutopilotUtility &autopilotUtility);
  virtual ~ManualAutopilot();

  /**
   * @brief nop. Bind the values and return.
   * @param controlSurfacePacket  The control surface packet to modify.
   * @param rcRxPacket The rc rx packet of stick input values.
   * @param imuPacket The imu packet of gyroscope and accelerometer values.
   */
  void trigger(common::ControlSurfacePacket &controlSurfacePacket,
               const common::RcRxPacket &rcRxPacket,
               const common::ImuPacket &imuPacket) override;

private:
  const AutopilotUtility &m_autopilotUtility;
};

}  // namespace autopilot
}  // namespace rcplane

#endif  // RCPLANE_AUTOPILOT_MANUAL_AUTOPILOT_HPP
