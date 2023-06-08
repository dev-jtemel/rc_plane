#include "rcplane/som/position/gps_controller.hpp"
#include "rcplane/common/io/journal.hpp"

namespace rcplane {
namespace som {
namespace position {

gps_controller::gps_controller() : ::rcplane::common::interface::base_controller("gps-controller") {
}

gps_controller::~gps_controller() {
}

bool gps_controller::init() {
  RCPLANE_LOG(info, _tag, "initialized");
  return false;
}

void gps_controller::start() {
  RCPLANE_LOG(info, _tag, "started");
}

void gps_controller::terminate() {
  RCPLANE_LOG(info, _tag, "terminated");
}

void gps_controller::register_cb(std::function<void()> cb) {
  _cbs.push_back(cb);
}


} // namesapce position
} // namesapce som
} // namesapce rcplane
