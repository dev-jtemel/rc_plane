#ifndef RCPLANE_AUTOPILOT_AUTOPILOT_INTERFACE_HPP
#define RCPLANE_AUTOPILOT_AUTOPILOT_INTERFACE_HPP

#include <boost/noncopyable.hpp>

#include "rcplane/common/Packet.hpp"
#include "rcplane/io/ConfigManager.hpp"

namespace rcplane {
namespace autopilot {

/**
 * @brief Interface for standardizing autopilot api and behaviour.
 */
class IAutopilot : public ::boost::noncopyable {
public:
  /**
   * @brief Execute the autopilot implementation.
   * @param controlSurfacePacket The control surface packet to modify.
   * @param rcRxPacket The received rc packet.
   * @param imuPacket The imu packet of gyroscope and accelerometer values.
   */
  virtual void trigger(common::ControlSurfacePacket &controlSurfacePacket,
                       const common::RcRxPacket &rcRxPacket,
                       const common::ImuPacket &imuPacket) = 0;

protected:
  /**
   * @brief Map the throttle rc value to a valid motor speed.
   * @param throttleStick Stick value from the rc controller [0, 100].
   * @return uint8_t The mapped motor speed.
   */
  uint8_t bindThrottleToRange(const uint8_t &throttleStick) {
    return bindToRange<uint8_t>(throttleStick, 0, 100, 0, 100);
  }

  /**
   * @brief Map the aileron rc value to a valid aileron deflection.
   * @param aileronStick Stick value from the rc controller [-100, 100].
   * @return int8_t The mapped aileron deflection.
   */
  int8_t bindAileronDeflectionToRange(const int8_t &aileronDeflection) {
    return bindToRange<int8_t>(aileronDeflection, -100, 100, -30, 30);
  }

  /**
   * @brief Map the elevator rc value to a valid elevator deflection.
   * @param elevatorDeflection Stick value from the rc controller [-100, 100].
   * @return int8_t The mapped elevator deflection.
   */
  int8_t bindElevatorDeflectionToRange(const int8_t &elevatorDeflection) {
    return bindToRange<int8_t>(elevatorDeflection, -100, 100, -40, 40);
  }

  /**
   * @brief Map the rudder rc value to a valid rudder deflection.
   * @param rudderDeflection Stick value from the rc controller [-100, 100].
   * @return int8_t The mapped rudder deflection.
   */
  int8_t bindRudderDeflectionToRange(const int8_t &rudderDeflection) {
    return bindToRange<int8_t>(rudderDeflection, -100, 100, -20, 20);
  }

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
  T bindToRange(const T value,
                const T inputMin,
                const T inputMax,
                const T outputMin,
                const T outputMax) const {
    return (value - inputMin) * (outputMax - outputMin) / (inputMax - inputMin)
        + outputMin;
  }
};

}  // namespace autopilot
}  // namespace rcplane

#endif  // RCPLANE_AUTOPILOT_AUTOPILOT_INTERFACE_HPP