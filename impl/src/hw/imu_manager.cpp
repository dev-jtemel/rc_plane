#include "rcplane/hw/imu_manager.hpp"
#include "rcplane/io/config_manager.hpp"
#include "rcplane/io/journal.hpp"

namespace rcplane {
namespace hw {

imu_manager::imu_manager() : interface::base_controller("imu-manager") {
  RCPLANE_ENTER();

  cROLL_MAX_POS =
      io::config_manager::instance().get<int8_t>("rcplane.hw.imu.roll_max_pos");
  cROLL_MAX_NEG =
      io::config_manager::instance().get<int8_t>("rcplane.hw.imu.roll_max_neg")
      * -1;
  cPTICH_MAX_POS = io::config_manager::instance().get<int8_t>(
      "rcplane.hw.imu.pitch_max_pos");
  cPTICH_MAX_NEG =
      io::config_manager::instance().get<int8_t>("rcplane.hw.imu.pitch_max_neg")
      * -1;
}
imu_manager::~imu_manager() { RCPLANE_ENTER(); }

bool imu_manager::init() {
  RCPLANE_ENTER();
  RCPLANE_LOG(info, _tag, "initialized");
  return true;
}

void imu_manager::start() {
  RCPLANE_ENTER();
  RCPLANE_LOG(info, _tag, "started");
}

void imu_manager::terminate() {
  RCPLANE_ENTER();
  RCPLANE_LOG(info, _tag, "terminated");
}

int8_t imu_manager::on(common::imu_packet *_imu_packet) {
  RCPLANE_ENTER();

  if (_imu_packet->roll > 30) {
    return -1;
  } else if (_imu_packet->roll < -30) {
    return 1;
  }
  return 0;
}

void imu_manager::roll_limiter(int8_t &roll) { RCPLANE_ENTER(); }

void imu_manager::pitch_limiter(int8_t &pitch) { RCPLANE_ENTER(); }

}  // namespace hw
}  // namespace rcplane