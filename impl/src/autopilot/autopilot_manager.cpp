#include "rcplane/autopilot/autopilot_manager.hpp"
#include "rcplane/io/journal.hpp"

namespace rcplane {
namespace autopilot {

autopilot_manager::autopilot_manager(boost::asio::io_context &io)
  : rcplane::interface::base_controller("autopilot_manager"), _io(io) {
  RCPLANE_ENTER();
}
autopilot_manager::~autopilot_manager() { RCPLANE_ENTER(); }

bool autopilot_manager::init() {
  RCPLANE_ENTER();

  _autopilot = &_ap_manual;
  _autopilot->on_entry();

  RCPLANE_LOG(info, _tag, "initialized");
  return true;
}

void autopilot_manager::start() {
  RCPLANE_ENTER();

  RCPLANE_LOG(info, _tag, "started");
}

void autopilot_manager::terminate() {
  RCPLANE_ENTER();

  _autopilot->on_exit();
  _autopilot = nullptr;

  RCPLANE_LOG(info, _tag, "terminated");
}

interface::autopilot_interface *autopilot_manager::autopilot() {
  RCPLANE_ENTER();
  return _autopilot;
}

void autopilot_manager::on_state_signal(common::state_packet *state_packet) {
  RCPLANE_ENTER();

  if ((_prev_state & 0xC0) != (state_packet->state & 0xC0)) {
    _autopilot->on_exit();
    _autopilot = state_packet->state & 0x80
        ? dynamic_cast<interface::autopilot_interface *>(&_ap_stabilize)
        : dynamic_cast<interface::autopilot_interface *>(&_ap_manual);
    _autopilot->on_entry();

    _prev_state = state_packet->state;
  }
}

void autopilot_manager::on_mcu_signal(common::control_surface_packet *cs_packet,
                                      common::imu_packet *imu_packet) {
  RCPLANE_ENTER();

  _io.post(
      boost::bind(&rcplane::autopilot::interface::autopilot_interface::handler,
                  _autopilot,
                  cs_packet,
                  imu_packet));
}

}  // namespace autopilot
}  // namespace rcplane