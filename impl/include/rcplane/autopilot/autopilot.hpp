#ifndef __RCPLANE__AUTOPILOT__AUTOPILOT_HPP__
#define __RCPLANE__AUTOPILOT__AUTOPILOT_HPP__

#include <boost/noncopyable.hpp>

#include "rcplane/base_controller.hpp"
#include "rcplane/common/packet.hpp"
#include "rcplane/hw/control_surface_manager.hpp"
#include "rcplane/hw/imu_manager.hpp"

namespace rcplane {
namespace autopilot {

/**
 * @brief Handle and manage imu data from the MCU.
 * 
 * The imu is used to predict and control the pose and position
 * of the plane.
 */
class autopilot : public interface::base_controller,
                  public ::boost::noncopyable {
public:
  /**
  * @brief Construct a new imu manager object.
  */
  explicit autopilot() : interface::base_controller("autopilot") {}
  ~autopilot() {}

  /**
   * @brief NOP.
   * @return true always.
   */
  bool init() override { return true; }

  /**
   * @brief NOP.
   */
  void start() override {}

  /**
   * @brief NOP.
   */
  void terminate() override {}

  /**
   * @brief Handle an incoming imu_packet, applying
   * and filters and/or limiters.
   * 
   * @param _imu_packet Pointer to the most recent packet received.
   */
  void on(common::control_surface_packet *_cs_packet,
          common::imu_packet *_imu_packet) {
    RCPLANE_ENTER();
    _cs_manager.on(_cs_packet, _imu_manager.on(_imu_packet));
  }

private:
  hw::control_surface_manager _cs_manager;
  hw::imu_manager _imu_manager;
};

}  // namespace autopilot
}  // namespace rcplane

#endif  //__RCPLANE__AUTOPILOT__AUTOPILOT_HPP__