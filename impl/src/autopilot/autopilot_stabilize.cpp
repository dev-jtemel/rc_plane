#include "rcplane/autopilot/autopilot_stabilize.hpp"

namespace rcplane {
namespace autopilot {

autopilot_stabilize::autopilot_stabilize()
  : interface::autopilot_interface("autopilot_stabilize") {
  RCPLANE_ENTER();
}
autopilot_stabilize::~autopilot_stabilize() { RCPLANE_ENTER(); }

void autopilot_stabilize::on_entry() {
  RCPLANE_ENTER();

  RCPLANE_LOG(info, _tag, "entering...");
}
void autopilot_stabilize::on_exit() {
  RCPLANE_ENTER();

  RCPLANE_LOG(info, _tag, "exiting...");
}

void autopilot_stabilize::handler_impl(
    common::control_surface_packet *cs_packet,
    common::imu_packet *imu_packet) {
  RCPLANE_ENTER();

  RCPLANE_LOG(info, _tag, "stabalizing...");
  //_cs_manager.on(cs_packet, _imu_manager.on(imu_packet));
}

}  // namespace autopilot
}  // namespace rcplane