#ifndef __RCPLANE__SOM__POSITION__GPS_CONTROLLER_HPP__
#define __RCPLANE__SOM__POSITION__GPS_CONTROLLER_HPP__

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

  void register_cb(std::function<void()> cb);
 
 private:
  std::vector<std::function<void()>> _cbs;
};

} // namesapce position
} // namesapce som
} // namesapce rcplane

#endif //__RCPLANE__SOM__POSITION__GPS_CONTROLLER_HPP__
