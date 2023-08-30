#include "rcplane/autopilot/StabilizeAutopilot.hpp"
#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace autopilot {

StabilizeAutopilot::StabilizeAutopilot(const AutopilotUtility &autopilotUtility)
  : IAutopilot(), m_autopilotUtility(autopilotUtility) {
  RCPLANE_LOG_METHOD();

/*
  c_kp = configManager.getValue<double>("rcplane.autopilot.stabilize.kp");
  c_ki = configManager.getValue<double>("rcplane.autopilot.stabilize.ki");
  c_kd = configManager.getValue<double>("rcplane.autopilot.stabilize.kd");
  c_maxIntegralError = configManager.getValue<double>(
      "rcplane.autopilot.stabilize.max_integral_error");
      */
}

StabilizeAutopilot::~StabilizeAutopilot() { RCPLANE_LOG_METHOD(); }

void StabilizeAutopilot::trigger(
    common::ControlSurfacePacket &controlSurfacePacket,
    const common::RcRxPacket &rcRxPacket,
    const common::ImuPacket &imuPacket) {
  RCPLANE_LOG_METHOD();

  double error = m_desiredRollAngle - imuPacket.gyroX;
  m_integralError += error;

  if (m_integralError > c_maxIntegralError) {
    m_integralError = c_maxIntegralError;
  } else if (m_integralError < -c_maxIntegralError) {
    m_integralError = -c_maxIntegralError;
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
      m_autopilotUtility.bindRcThrottle(rcRxPacket.motorStickPosition);
  controlSurfacePacket.aileronDeflection = static_cast<int8_t>(output);
  controlSurfacePacket.elevatorDeflection =
      m_autopilotUtility.bindRcElevatorDeflection(rcRxPacket.elevatorStickPosition);
  controlSurfacePacket.rudderDeflection =
      m_autopilotUtility.bindRcRudderDeflection(rcRxPacket.rudderStickPosition);
}

}  // namespace autopilot
}  // namespace rcplane