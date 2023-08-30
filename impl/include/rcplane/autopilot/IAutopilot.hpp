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
};

}  // namespace autopilot
}  // namespace rcplane

#endif  // RCPLANE_AUTOPILOT_AUTOPILOT_INTERFACE_HPP