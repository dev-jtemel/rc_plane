/**
 * @file autopilot_interface.hpp
 * @author Jonathon Temelkovski (dev.jtemel@gmail.com)
 * @version 0.1
 * @date 2023-08-21
 */
#ifndef __RCPLANE__AUTOPILOT__AUTOPILOT_INTERFACE_HPP__
#define __RCPLANE__AUTOPILOT__AUTOPILOT_INTERFACE_HPP__

#include <boost/noncopyable.hpp>
#include <boost/signals2.hpp>

#include "rcplane/common/packet.hpp"
#include "rcplane/fsm_base_controller.hpp"
#include "rcplane/hw/control_surface_manager.hpp"
#include "rcplane/hw/imu_manager.hpp"

namespace rcplane {
namespace autopilot {
namespace interface {

/**
 * @brief Interface to post events to an underlying autopilot system.
 * 
 * All autopilot systems must implement this class.
 */
class autopilot_interface : public rcplane::interface::fsm_base_controller,
                            public ::boost::noncopyable {
public:
  /**
   * @brief Construct a new autopilot interface object.
   * 
   * @param tag The logging tag of the underlying autopilot system.
   * @param config_manager The config manager to grab config values from.
   */
  explicit autopilot_interface(std::string tag,
                               rcplane::io::config_manager &config_manager);
  ~autopilot_interface();

  /**
   * @brief Get the write_signal signal to connect to.
   * @return boost::signals2::signal<void()>& The underlying signal handler.
   */
  boost::signals2::signal<void()> &write_signal();

  /**
   * @brief Take ownership of the write_signal handler.
   * @param write_signal The signal handler to take ownership of.
   */
  void set_write_signal(boost::signals2::signal<void()> &write_signal);

  /**
   * @brief Post an io event to handle the incoming packets.
   * 
   * This internally calls handler_impl() allow the dervied autopilot system to
   * handle the packets accordingly. 
   * 
   * Fires _write_signal() on exiting.
   * 
   * @param cs_packet The control_surface_packet to handle.
   * @param imu_packet The imu_packet to handle.
   */
  void handler(common::control_surface_packet *cs_packet,
               common::imu_packet *imu_packet);

  /**
   * @brief Handle the incoming packets. Must be implemented in derived class.
   * 
   * @param cs_packet The control_surface_packet to handle.
   * @param imu_packet The imu_packet to handle.
   */
  virtual void handler_impl(common::control_surface_packet *cs_packet,
                            common::imu_packet *imu_packet) = 0;

protected:
  hw::control_surface_manager _cs_manager;
  hw::imu_manager _imu_manager;
  boost::signals2::signal<void()> _write_signal;
};

}  // namespace interface
}  // namespace autopilot
}  // namespace rcplane

#endif  //__RCPLANE__AUTOPILOT__AUTOPILOT_INTERFACE_HPP__