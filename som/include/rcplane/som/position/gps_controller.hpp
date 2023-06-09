#ifndef __RCPLANE__SOM__POSITION__GPS_CONTROLLER_HPP__
#define __RCPLANE__SOM__POSITION__GPS_CONTROLLER_HPP__

#include <gps.h>
#include <functional>

#include "rcplane/common/base_controller.hpp"

namespace rcplane {
namespace som {
namespace position {

class gps_controller : public ::rcplane::common::interface::base_controller {
 public:
  gps_controller();
  ~gps_controller();

  bool init() override;
  void start() override;
  void terminate() override;

  void register_cb(std::function<void(float, float)> cb);
 
 private:
  void p_read_gps();

  char *GPSD_PORT = "2000";
  size_t GPS_DELAY = 5000000;
  static size_t MODE_STR_NUM = 4;
  std::string MODE_STR[MODE_STR_NUM] = {"n/a", "None", "2D", "3D" };
  std::vector<std::function<void(float, float)>> _cbs;
  struct gps_data_t _gps_data;
};

} // namesapce position
} // namesapce som
} // namesapce rcplane

#endif //__RCPLANE__SOM__POSITION__GPS_CONTROLLER_HPP__
