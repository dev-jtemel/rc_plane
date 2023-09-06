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
   *
   * This function performs PID control on both the roll and pitch angle, with internal
   * integral and derivative errors held exclusively.
   *
   * PID control applies the following formula on each trigger call:
   * \f$e_a(t) = K_pe(t) + K_d \frac{d}{dt}e(t) + K_i \int e(t)dt\f$.
   *
   * with Laplace Transform:
   * \f$E_a(s) = E(s) \big[ K_p + sK_d + \frac{K_i}{s} \big]\f$
   *
   * where
   *
   * \f$K_p = \f$ c_kp<br>
   * \f$K_d = \f$ c_kd<br>
   * \f$K_i = \f$ c_ki
   */
  void trigger(common::ControlSurfacePacket &controlSurfacePacket,
               const common::RcRxPacket &rcRxPacket,
               const common::ImuPacket &imuPacket) override;

private:
  /**
   * @brief Apply PID control to the roll of the plane.
   * @param rollAngle The current roll angle of the plane.
   * @return int8_t The amount to deflect the ailerons.
   */
  int8_t computeRollToAileronDeflection(const double &rollAngle);

  /**
   * @brief Apply PID control to the pitch of the plane.
   * @param pitchAngle The current pitch angle of the plane.
   * @return int8_t The amount to deflect the elevators.
   */
  int8_t computePitchToElevatorDeflection(const double &pitchAngle);

  /**
   * @brief Bind the integral error within the configured range.
   * @param integralError The integral error.
   * @return int8_t The bound integral error.
   */
  int8_t bindIntegralError(const int8_t &integralError) const;

  double c_maxAileronDeflection{0.0};
  double c_kp{};
  double c_ki{};
  double c_kd{};
  double c_tau{};
  double c_maxIntegralError{};

  double m_timeStamp{0.0};
  double m_prevRollError{0.0};
  double m_prevPitchError{0.0};
  double m_rollDerivativeError{0.0};
  double m_rollIntegralError{0.0};
  double m_pitchIntegralError{0.0};

  double m_desiredRollAngle{0.0};
  double m_desiredPitchAngle{0.0};

  const AutopilotUtility &m_autopilotUtility;
};

}  // namespace autopilot
}  // namespace rcplane

#endif  // RCPLANE_AUTOPILOT_STABILIZE_AUTOPILOT_HPP
