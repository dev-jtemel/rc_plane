/**
 * @file autopilot_stabilize.hpp
 * @author Jonathon Temelkovski (dev.jtemel@gmail.com)
 * @version 0.1
 * @date 2023-08-21
 */
#ifndef __RCPLANE__AUTOPILOT__AUTOPILOT_STABILIZE_HPP__
#define __RCPLANE__AUTOPILOT__AUTOPILOT_STABILIZE_HPP__

#include "rcplane/autopilot/autopilot_interface.hpp"

namespace rcplane {
namespace autopilot {

/**
 * @brief Autopilot system that performs roll and pitch stabilization.
 */
class autopilot_stabilize : public interface::autopilot_interface {
public:
  /**
   * @brief Construct a new autopilot stabilize object
   */
  explicit autopilot_stabilize();
  ~autopilot_stabilize();

  /**
   * @brief nop.
   */
  void on_entry() override;

  /**
   * @brief nop.
   */
  void on_exit() override;

  /**
   * @brief Use the imu data to adjust ailerons and/or elevators to return/maintain
   * neuteral flight.
   * 
   * @param cs_packet The incoming control_surface_packet.
   * @param imu_packet The incoming imu_packet.
   */
  void handler_impl(common::control_surface_packet *cs_packet,
                    common::imu_packet *imu_packet) override;
};

}  // namespace autopilot
}  // namespace rcplane

#endif  //__RCPLANE__AUTOPILOT__AUTOPILOT_STABILIZE_HPP__