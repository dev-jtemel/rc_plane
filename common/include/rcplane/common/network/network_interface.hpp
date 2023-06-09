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

  void gps_cb(float lt, float ln, float track) {
    std::lock_guard<std::mutex> lk(_gps_lk);
    _gps = std::make_tuple(lt, ln, track);
  }

 protected:
  std::function<void(int)> _termination_handler;

  std::mutex _gps_lk;
  std::tuple<float, float, float> _gps;
};

} // namespace interface
} // namespace network
} // namespace common
} // namespace rcplane

#endif //__RCPLANE__COMMON__NETWORK__NETWORK_INTERFACE_HPP__

