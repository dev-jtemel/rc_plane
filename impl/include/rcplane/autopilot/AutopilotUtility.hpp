#ifndef RCPLANE_AUTOPILOT_AUTOPILOT_UTILITY_HPP
#define RCPLANE_AUTOPILOT_AUTOPILOT_UTILITY_HPP

#include <boost/noncopyable.hpp>

#include "rcplane/io/ConfigManager.hpp"

namespace rcplane {
namespace autopilot {

/**
 * @brief Maintain config values and helper functions for autopilot systems.
 */
class AutopilotUtility : public ::boost::noncopyable {
public:
  /**
   * @brief Construct a new AutopilotUtility and supported autopilot systems
   * (ManualAutopilot is default).
   * @param configManager The configuration manager.
   */
  AutopilotUtility(const io::ConfigManager &configManager);
  ~AutopilotUtility();

  /**
   * @brief Bind the rc stick position to the configured output range.
   * @param value The value of the rc stick position.
   * @return uint8_t The bounded value.
   */
  uint8_t bindRcThrottle(const uint8_t &value) const;

  /**
   * @brief Bind the rc aileron position to the configured output range.
   * @param value The value of the rc aileron position.
   * @return uint8_t The bounded value.
   */
  int8_t bindRcAileronDeflection(const int8_t &value) const;

  /**
   * @brief Bind the rc elevator position to the configured output range.
   * @param value The value of the rc elevator position.
   * @return uint8_t The bounded value.
   */
  int8_t bindRcElevatorDeflection(const int8_t &value) const;

  /**
   * @brief Bind the rc rudder position to the configured output range.
   * @param value The value of the rc rudder position.
   * @return uint8_t The bounded value.
   */
  int8_t bindRcRudderDeflection(const int8_t &value) const;

private:
  /**
   * @brief Helper function to map a value from one range to another.
   * @tparam T The type of the value to map.
   * @param value The value to map.
   * @param inputMin The minimum value of the input range.
   * @param inputMax The maximum value of the input range.
   * @param outputMin The minimum value of the output range.
   * @param outputMax The maximum value of the output range.
   * @return T 
   */
  template<typename T>
  inline T bindToRange(const T value,
                const T inputMin,
                const T inputMax,
                const T outputMin,
                const T outputMax) const {
    return (value - inputMin) * (outputMax - outputMin) / (inputMax - inputMin)
        + outputMin;
  }

  uint8_t c_maxThrottle{};
  int8_t c_maxAileronDeflection{};
  int8_t c_maxElevatorDeflection{};
  int8_t c_maxRudderDeflection{};
};

}  // namespace autopilot
}  // namespace rcplane

#endif  //RCPLANE_AUTOPILOT_AUTOPILOT_UTILITY_HPP