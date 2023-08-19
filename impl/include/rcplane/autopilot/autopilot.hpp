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
  explicit autopilot(boost::asio::io_context &io)
    : interface::base_controller("autopilot"), _io(io) {
    RCPLANE_ENTER();
  }
  ~autopilot() { RCPLANE_ENTER(); }

  /**
   * @brief NOP.
   * @return true always.
   */
  bool init() override {
    RCPLANE_ENTER();
    (void)_cs_manager.init();
    (void)_imu_manager.init();
    RCPLANE_LOG(info, _tag, "initialized");
    return true;
  }

  /**
   * @brief NOP.
   */
  void start() override {
    RCPLANE_ENTER();
    _cs_manager.start();
    _imu_manager.start();
    RCPLANE_LOG(info, _tag, "started");
  }

  /**
   * @brief NOP.
   */
  void terminate() override {
    RCPLANE_ENTER();
    _cs_manager.terminate();
    _imu_manager.terminate();
    RCPLANE_LOG(info, _tag, "terminated");
  }

  /**
   * @brief Post the packet to the io service to handle in main thread.
   * @param _cs_packet Pointer to the most recent control_surface_packet received.
   * @param _imu_packet Pointer to the most recent imu_packet received.
   */
  void on(common::control_surface_packet *_cs_packet,
          common::imu_packet *_imu_packet) {
    RCPLANE_ENTER();
    _io.post(boost::bind(&autopilot::handler, this, _cs_packet, _imu_packet));
  }

private:
  /**
   * @brief Handle an incoming imu_packet, applying
   * and filters and/or limiters.
   * 
   * Lifespan of underlying packets are maintained in the serial_controller.
   * 
   * @see serial_controller
   * @param _cs_packet Pointer to the most recent control_surface_packet received.
   * @param _imu_packet Pointer to the most recent imu_packet received.
   */
  void handler(common::control_surface_packet *_cs_packet,
               common::imu_packet *_imu_packet) {
    RCPLANE_ENTER();
    _cs_manager.on(_cs_packet, _imu_manager.on(_imu_packet));
    ;
  }

  boost::asio::io_context &_io;
  hw::control_surface_manager _cs_manager;
  hw::imu_manager _imu_manager;
};

}  // namespace autopilot
}  // namespace rcplane

#endif  //__RCPLANE__AUTOPILOT__AUTOPILOT_HPP__