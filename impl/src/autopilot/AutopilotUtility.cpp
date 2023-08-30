#include "rcplane/autopilot/AutopilotUtility.hpp"

#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace autopilot {

AutopilotUtility::AutopilotUtility(const io::ConfigManager &configManager)
  : m_configManager(configManager) {
  RCPLANE_LOG_METHOD();

  c_maxThrottle =
      configManager.getValue<uint8_t>("rcplane.autopilot.max_throttle");
  c_maxAileronDeflection = configManager.getValue<int8_t>(
      "rcplane.autopilot.max_aileron_deflection");
  c_maxElevatorDeflection = configManager.getValue<int8_t>(
      "rcplane.autopilot.max_elevator_deflection");
  c_maxRudderDeflection =
      configManager.getValue<int8_t>("rcplane.autopilot.max_rudder_deflection");
}
AutopilotUtility::~AutopilotUtility() { RCPLANE_LOG_METHOD(); }

const io::ConfigManager &AutopilotUtility::getConfigManager() const {
  RCPLANE_LOG_METHOD();
  return m_configManager;
}

uint8_t AutopilotUtility::bindRcThrottle(const uint8_t &value) const {
  RCPLANE_LOG_METHOD();

  return bindToRange<uint8_t>(value, 0, 100, -c_maxThrottle, c_maxThrottle);
}

int8_t AutopilotUtility::bindRcAileronDeflection(const int8_t &value) const {
  RCPLANE_LOG_METHOD();

  return bindToRange<int8_t>(value,
                             -100,
                             100,
                             -c_maxAileronDeflection,
                             c_maxAileronDeflection);
}

int8_t AutopilotUtility::bindRcElevatorDeflection(const int8_t &value) const {
  RCPLANE_LOG_METHOD();

  return bindToRange<int8_t>(value,
                             -100,
                             100,
                             -c_maxElevatorDeflection,
                             c_maxElevatorDeflection);
}

int8_t AutopilotUtility::bindRcRudderDeflection(const int8_t &value) const {
  RCPLANE_LOG_METHOD();

  return bindToRange<int8_t>(value,
                             -100,
                             100,
                             -c_maxRudderDeflection,
                             c_maxRudderDeflection);
}

int8_t AutopilotUtility::bindPidAileronDeflection(const int8_t &value) const {
  RCPLANE_LOG_METHOD();

  if (value > c_maxAileronDeflection) { return c_maxAileronDeflection; }
  if (value < -c_maxAileronDeflection) { return -c_maxAileronDeflection; }
  return value;
}

int8_t AutopilotUtility::bindPidElevatorDeflection(const int8_t &value) const {
  RCPLANE_LOG_METHOD();

  if (value > c_maxElevatorDeflection) { return c_maxElevatorDeflection; }
  if (value < -c_maxElevatorDeflection) { return -c_maxElevatorDeflection; }
  return value;
}

}  // namespace autopilot
}  // namespace rcplane