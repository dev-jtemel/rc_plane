/**
 * @file autopilot_manual.cpp
 * @author Jonathon Temelkovski (dev.jtemel@gmail.com)
 * @version 0.1
 * @date 2023-08-21
 */
#include "rcplane/autopilot/autopilot_manual.hpp"
#include "rcplane/io/journal.hpp"

namespace rcplane {
namespace autopilot {

autopilot_manual::autopilot_manual(rcplane::io::config_manager &config_manager)
  : interface::autopilot_interface("autopilot_manual", config_manager) {
  RCPLANE_ENTER();
}
autopilot_manual::~autopilot_manual() { RCPLANE_ENTER(); }

void autopilot_manual::on_entry() {
  RCPLANE_ENTER();

  RCPLANE_LOG(info, _tag, "entering...");
}
void autopilot_manual::on_exit() {
  RCPLANE_ENTER();

  RCPLANE_LOG(info, _tag, "exiting...");
}

void autopilot_manual::handler_impl(common::control_surface_packet *cs_packet,
                                    common::imu_packet *imu_packet) {
  RCPLANE_ENTER();

  RCPLANE_LOG(info, _tag, "nop");
}

}  // namespace autopilot
}  // namespace rcplane