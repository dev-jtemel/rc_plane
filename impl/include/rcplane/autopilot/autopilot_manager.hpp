/**
 * @file autopilot_manager.hpp
 * @author Jonathon Temelkovski (dev.jtemel@gmail.com)
 * @version 0.1
 * @date 2023-08-21
 */
#ifndef __RCPLANE__AUTOPILOT__AUTOPILOT_MANAGER_HPP__
#define __RCPLANE__AUTOPILOT__AUTOPILOT_MANAGER_HPP__

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include "rcplane/autopilot/autopilot_interface.hpp"
#include "rcplane/autopilot/autopilot_manual.hpp"
#include "rcplane/autopilot/autopilot_stabilize.hpp"
#include "rcplane/base_controller.hpp"
#include "rcplane/common/packet.hpp"
#include "rcplane/io/config_manager.hpp"

namespace rcplane {
namespace autopilot {

/**
 * @brief Maintain, transition and fire the correct autopilot system.
 * 
 * This class handles incoming state_packets to apply the correct autopilot
 * system requested. 
 */
class autopilot_manager : public rcplane::interface::base_controller,
                          public ::boost::noncopyable {
public:
  /**
   * @brief Construct a new autopilot manager object.
   * 
   * @param config_manager The config manager to grab config values from.
   * @param io Shared io context.
   */
  explicit autopilot_manager(rcplane::io::config_manager &config_manager,
                             boost::asio::io_context &io);
  ~autopilot_manager();

  /**
   * @brief Initialize the autopilot to autopilot_manual by default.
   * @return bool Always true.
   */
  bool init() override;

  /**
   * @brief nop.
   */
  void start() override;

  /**
   * @brief nop.
   */
  void terminate() override;

  /**
   * @brief Get the current autopilot system.
   * 
   * @return interface::autopilot_interface* Current autopilot system.
   */
  interface::autopilot_interface *autopilot();

  /**
   * @brief Transition (or not) to the requested autopilot system.
   * 
   * @param state_packet The incoming state_packet.
   */
  void on_state_signal(common::state_packet *state_packet);

  /**
   * @brief Post the incoming mcu packets to the underlying autopilot system.
   * 
   * @param cs_packet Incoming control_surface_packet.
   * @param imu_packet Incoming imu_packet.
   */
  void on_mcu_signal(common::control_surface_packet *cs_packet,
                     common::imu_packet *imu_packet);

protected:
  boost::asio::io_context &_io;

private:
  uint8_t _prev_state{};
  interface::autopilot_interface *_autopilot{nullptr};
  autopilot_manual _ap_manual;
  autopilot_stabilize _ap_stabilize;
};

}  // namespace autopilot
}  // namespace rcplane

#endif  //__RCPLANE__AUTOPILOT__AUTOPILOT_MANAGER_HPP__