#include "rcplane/som/position/gps_controller.hpp"
#include "rcplane/common/io/config_manager.hpp"
#include "rcplane/common/io/journal.hpp"
#include <cmath>
#include <sstream>

namespace rcplane {
namespace som {
namespace position {

gps_controller::gps_controller()
  : ::rcplane::common::interface::base_controller("gps-controller") {
  RCPLANE_ENTER(_tag);

  GPSD_PORT = common::io::config_manager::instance().get<std::string>(
      "som.gps_controller.gpsd.port");
  GPSD_DELAY = common::io::config_manager::instance().get<size_t>(
      "som.gps_controller.gpsd.delay");
}

gps_controller::~gps_controller() { RCPLANE_ENTER(_tag); }

bool gps_controller::init() {
  RCPLANE_ENTER(_tag);

  if (gps_open("localhost", GPSD_PORT.c_str(), &_gps_data)) {
    RCPLANE_LOG(error, _tag, "failed to open gpsd");
    return false;
  }

  RCPLANE_LOG(info, _tag, "initialized");
  return true;
}

void gps_controller::start() {
  RCPLANE_ENTER(_tag);

  (void)gps_stream(&_gps_data, WATCH_ENABLE | WATCH_JSON, NULL);

  _running = true;
  _worker = boost::thread(&gps_controller::p_read_gps, this);

  RCPLANE_LOG(info, _tag, "started");
}

void gps_controller::terminate() {
  RCPLANE_ENTER(_tag);

  _running = false;
  _worker.join();

  (void)gps_stream(&_gps_data, WATCH_DISABLE, NULL);
  (void)gps_close(&_gps_data);

  RCPLANE_LOG(info, _tag, "terminated");
}

void gps_controller::register_cb(
    std::function<void(float, float, float, float)> cb) {
  RCPLANE_ENTER(_tag);

  _cbs.push_back(cb);
}

void gps_controller::p_read_gps() {
  RCPLANE_ENTER(_tag);

  while (_running && gps_waiting(&_gps_data, GPSD_DELAY)) {
    if (0 > gps_read(&_gps_data, NULL, 0)) {
      RCPLANE_LOG(error, _tag, "invalid read");
      break;
    }
    if (MODE_SET != (MODE_SET & _gps_data.set)) { continue; }
    if (0 > _gps_data.fix.mode || MODE_STR_NUM <= _gps_data.fix.mode) {
      _gps_data.fix.mode = 0;
    }

    std::ostringstream os;
    os << "fix: " << MODE_STR[_gps_data.fix.mode];

    if (std::isfinite(_gps_data.fix.latitude)
        && std::isfinite(_gps_data.fix.longitude)
        && std::isfinite(_gps_data.fix.track)) {
      auto latitude = _gps_data.fix.latitude;
      auto longitude = _gps_data.fix.longitude;
      auto track = _gps_data.fix.track;
      auto speed = _gps_data.fix.speed;
      os << " lat: " << latitude << " lon: " << longitude << " track: " << track
         << " speed: " << speed;

      for (auto cb : _cbs) { cb(latitude, longitude, track, speed); }
    }

    RCPLANE_LOG(trace, _tag, os.str());
  }
}

}  // namespace position
}  // namespace som
}  // namespace rcplane
