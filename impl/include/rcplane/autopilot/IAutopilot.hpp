#ifndef RCPLANE_AUTOPILOT_AUTOPILOT_INTERFACE_HPP
#define RCPLANE_AUTOPILOT_AUTOPILOT_INTERFACE_HPP

#include <boost/noncopyable.hpp>

#include "rcplane/common/Packet.hpp"
#include "rcplane/io/ConfigManager.hpp"

namespace rcplane {
namespace autopilot {

class IAutopilot : public ::boost::noncopyable {
public:
  virtual void trigger(common::ControlSurfacePacket &controlSurfacePacket,
                       const common::RcRxPacket &rcRxPacket) = 0;

protected:
  uint8_t bindThrottleToRange(const uint8_t &throttleStick) {
    return bindToRange<uint8_t>(throttleStick, 0, 100, 0, 100);
  }

  int8_t bindAileronDeflectionToRange(const int8_t &aileronDeflection) {
    return bindToRange<int8_t>(aileronDeflection, -100, 100, -30, 30);
  }

  int8_t bindElevatorDeflectionToRange(const int8_t &elevatorDeflection) {
    return bindToRange<int8_t>(elevatorDeflection, -100, 100, -40, 40);
  }

  int8_t bindRudderDeflectionToRange(const int8_t &rudderDeflection) {
    return bindToRange<int8_t>(rudderDeflection, -100, 100, -20, 20);
  }

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