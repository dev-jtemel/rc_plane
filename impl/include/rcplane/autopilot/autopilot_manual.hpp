/**
 * @file autopilot_manual.hpp
 * @author Jonathon Temelkovski (dev.jtemel@gmail.com)
 * @version 0.1
 * @date 2023-08-21
 */
#ifndef __RCPLANE__AUTOPILOT__AUTOPILOT_MANUAL_HPP__
#define __RCPLANE__AUTOPILOT__AUTOPILOT_MANUAL_HPP__

#include "rcplane/autopilot/autopilot_interface.hpp"

namespace rcplane {
namespace autopilot {

/**
 * @brief NOP autopilot system. Used for 100% manual flying.
 */
class autopilot_manual : public interface::autopilot_interface {
public:
  /**
   * @brief Construct a new autopilot_manual object.
   * @param config_manager The config manager to grab config values from.
   */
  autopilot_manual(rcplane::io::config_manager &config_manager);
  ~autopilot_manual();

  /**
   * @brief nop.
   */
  void on_entry() override;

  /**
   * @brief nop.
   */
  void on_exit() override;

  /**
   * @brief nop.
   */
  void handler_impl(common::control_surface_packet *cs_packet,
                    common::imu_packet *imu_packet) override;
};

}  // namespace autopilot
}  // namespace rcplane

#endif  //__RCPLANE__AUTOPILOT__AUTOPILOT_MANUAL_HPP__