#include "rcplane/autopilot/StabilizeAutopilot.hpp"
#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace autopilot {

StabilizeAutopilot::StabilizeAutopilot(const AutopilotUtility &autopilotUtility)
  : IAutopilot(), m_autopilotUtility(autopilotUtility) {
  RCPLANE_LOG_METHOD();

  const auto &kConfigManager = m_autopilotUtility.getConfigManager();

  c_maxAileronDeflection = kConfigManager.getValue<double>(
      "rcplane.autopilot.max_aileron_deflection");
  c_kp = kConfigManager.getValue<double>("rcplane.autopilot.stabilize.kp");
  c_ki = kConfigManager.getValue<double>("rcplane.autopilot.stabilize.ki");
  c_kd = kConfigManager.getValue<double>("rcplane.autopilot.stabilize.kd");
  c_tau = kConfigManager.getValue<double>("rcplane.autopilot.stabilize.tau");
  c_maxIntegralError = kConfigManager.getValue<double>(
      "rcplane.autopilot.stabilize.max_integral_error");
}

StabilizeAutopilot::~StabilizeAutopilot() { RCPLANE_LOG_METHOD(); }

void StabilizeAutopilot::trigger(
    common::ControlSurfacePacket &controlSurfacePacket,
    const common::RcRxPacket &rcRxPacket,
    const common::ImuPacket &imuPacket) {
  RCPLANE_LOG_METHOD();

  controlSurfacePacket.motorSpeed =
      m_autopilotUtility.bindRcThrottle(rcRxPacket.motorStickPosition);
  controlSurfacePacket.aileronDeflection =
      computeRollToAileronDeflection(imuPacket.gyroX);
  controlSurfacePacket.elevatorDeflection =
      m_autopilotUtility.bindPidElevatorDeflection(
          computePitchToElevatorDeflection(imuPacket.gyroY));
  controlSurfacePacket.rudderDeflection =
      m_autopilotUtility.bindRcRudderDeflection(rcRxPacket.rudderStickPosition);
}

int8_t StabilizeAutopilot::computeRollToAileronDeflection(
    const double &rollAngle) {
  RCPLANE_LOG_METHOD();

  constexpr double timestamp = 0.04;  // dt of loop cycle
  double error = m_desiredRollAngle - rollAngle;

  m_rollIntegralError += (timestamp / 2) * (error + m_prevRollError);
  m_rollDerivativeError =
      (2 * c_tau - timestamp) / (2 * c_tau + timestamp) * m_rollDerivativeError
      + 2 / (2 * c_tau + timestamp) * (error - m_prevRollError);

  m_prevRollError = error;

  RCPLANE_LOG(debug,
              "error = " << error
                         << ", integral error = " << m_rollIntegralError
                         << ", derivative error = " << m_rollDerivativeError);

  double output =
      c_kp * error + c_ki * m_rollIntegralError + c_kd * m_rollDerivativeError;
  double unsaturatedOutput = output;

  output = m_autopilotUtility.bindPidAileronDeflection(output);

  m_rollIntegralError =
      m_rollIntegralError + timestamp / c_ki * (output - unsaturatedOutput);
  return static_cast<int8_t>(output);
}

int8_t StabilizeAutopilot::computePitchToElevatorDeflection(
    const double &pitchAngle) {
  RCPLANE_LOG_METHOD();

  double error = m_desiredPitchAngle - pitchAngle;
  double derivative = error - m_prevPitchError;
  m_pitchIntegralError += error;
  m_pitchIntegralError = bindIntegralError(m_pitchIntegralError);

  double output =
      c_kp * error + c_ki * m_pitchIntegralError + c_kd * derivative;
  m_prevPitchError = error;

  return static_cast<int8_t>(output);
}

int8_t StabilizeAutopilot::bindIntegralError(
    const int8_t &integralError) const {
  RCPLANE_LOG_METHOD();

  if (integralError > c_maxIntegralError) { return c_maxIntegralError; }
  if (integralError < -c_maxIntegralError) { return -c_maxIntegralError; }
  return integralError;
}

}  // namespace autopilot
}  // namespace rcplane