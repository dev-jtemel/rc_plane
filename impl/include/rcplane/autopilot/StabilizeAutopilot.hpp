#ifndef RCPLANE_AUTOPILOT_STABILIZE_AUTOPILOT_HPP
#define RCPLANE_AUTOPILOT_STABILIZE_AUTOPILOT_HPP

#include <boost/noncopyable.hpp>

#include "rcplane/autopilot/AutopilotUtility.hpp"
#include "rcplane/autopilot/IAutopilot.hpp"

namespace rcplane {
namespace autopilot {

/**
 * @brief Stabilize the plane while flying straight. Does NOT override user inputs.
 */
class StabilizeAutopilot : public IAutopilot {
public:
  StabilizeAutopilot(const AutopilotUtility &autopilotUtility);
  virtual ~StabilizeAutopilot();

  /**
   * @brief Stabilize the plane while flying straight. Does NOT override user inputs.
   * @param controlSurfacePacket  The control surface packet to modify.
   * @param rcRxPacket The rc rx packet of stick input values.
   * @param imuPacket The imu packet of gyroscope and accelerometer values.
   */
  void trigger(common::ControlSurfacePacket &controlSurfacePacket,
               const common::RcRxPacket &rcRxPacket,
               const common::ImuPacket &imuPacket);

private:
  double c_kp{};
  double c_ki{};
  double c_kd{};
  double c_maxIntegralError{};

  double m_timeStamp{0.0};
  double m_prevError{0.0};
  double m_integralError{0.0};
  double m_desiredRollAngle{0.0};

  const AutopilotUtility &m_autopilotUtility;
};

}  // namespace autopilot
}  // namespace rcplane

#endif  // RCPLANE_AUTOPILOT_STABILIZE_AUTOPILOT_HPP