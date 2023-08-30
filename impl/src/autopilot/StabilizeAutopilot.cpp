#include "rcplane/autopilot/StabilizeAutopilot.hpp"
#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace autopilot {

StabilizeAutopilot::StabilizeAutopilot() : IAutopilot() {
  RCPLANE_LOG_METHOD();
}

StabilizeAutopilot::~StabilizeAutopilot() { RCPLANE_LOG_METHOD(); }

void StabilizeAutopilot::trigger(
    common::ControlSurfacePacket &controlSurfacePacket,
    const common::RcRxPacket &rcRxPacket,
    const common::ImuPacket &imuPacket) {
  RCPLANE_LOG_METHOD();

  double error = m_desiredRollAngle - imuPacket.gyroX;
  m_integralError += error;

  if (m_integralError > 5) {
    m_integralError = 5;
  } else if (m_integralError < -5) {
    m_integralError = -5;
  }

  double derivativeError = (error - m_prevError);

  double output =
      (c_kp * error) + (c_ki * m_integralError) + (c_kd * derivativeError);

  m_prevError = error;

  if (output > 30) {
    output = 30;
  } else if (output < -30) {
    output = -30;
  }

  RCPLANE_LOG(warning, output);

  controlSurfacePacket.motorSpeed =
      bindThrottleToRange(rcRxPacket.motorStickPosition);
  controlSurfacePacket.aileronDeflection = static_cast<int8_t>(output);
  controlSurfacePacket.elevatorDeflection =
      bindElevatorDeflectionToRange(rcRxPacket.elevatorStickPosition);
  controlSurfacePacket.rudderDeflection =
      bindRudderDeflectionToRange(rcRxPacket.rudderStickPosition);
}

}  // namespace autopilot
}  // namespace rcplane