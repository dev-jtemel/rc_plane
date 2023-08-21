/**
 * @file autopilot_interface.cpp
 * @author Jonathon Temelkovski (dev.jtemel@gmail.com)
 * @version 0.1
 * @date 2023-08-21
 */
#include "rcplane/autopilot/autopilot_interface.hpp"
#include "rcplane/io/journal.hpp"

namespace rcplane {
namespace autopilot {
namespace interface {

autopilot_interface::autopilot_interface(std::string tag)
  : rcplane::interface::fsm_base_controller(tag) {
  RCPLANE_ENTER();
}
autopilot_interface::~autopilot_interface() { RCPLANE_ENTER(); }

boost::signals2::signal<void()> &autopilot_interface::write_signal() {
  RCPLANE_ENTER();
  
  return _write_signal;
}

void autopilot_interface::handler(common::control_surface_packet *cs_packet,
                                  common::imu_packet *imu_packet) {
  RCPLANE_ENTER();

  handler_impl(cs_packet, imu_packet);
  _write_signal();
}

}  // namespace interface
}  // namespace autopilot
}  // namespace rcplane