#ifndef __RCPLANE__COMMON__NETWORK__NETWORK_INTERFACE_HPP__
#define __RCPLANE__COMMON__NETWORK__NETWORK_INTERFACE_HPP__

#include <functional>
#include <mutex>
#include <tuple>

#include "rcplane/common/base_controller.hpp"

namespace rcplane {
namespace common {
namespace network {
namespace interface {

class network_interface : public ::rcplane::common::interface::base_controller {
 public:
  explicit network_interface(std::function<void(int)> termination_handler) 
    : base_controller("network_interface"), _termination_handler(termination_handler) {
  }

  virtual ~network_interface() = default;

  virtual bool init() = 0;
  virtual void start() = 0;
  virtual void terminate() = 0;

  void gps_cb(float lt, float ln, float track, float speed) {
    std::lock_guard<std::mutex> lk(_gps_lk);
    _gps = std::make_tuple(lt, ln, track, speed);
  }

  void cs_cb(int state, int motor, int aileron, int elevator, int rudder) {
    std::lock_guard<std::mutex> lk(_cs_lk);
    _cs = std::make_tuple(state, motor, aileron, elevator, rudder);
  }

 protected:
  std::function<void(int)> _termination_handler;

  std::mutex _gps_lk;
  std::tuple<float, float, float, float> _gps;

  std::mutex _cs_lk;
  std::tuple<int, int, int, int, int> _cs;
};

} // namespace interface
} // namespace network
} // namespace common
} // namespace rcplane

#endif //__RCPLANE__COMMON__NETWORK__NETWORK_INTERFACE_HPP__

