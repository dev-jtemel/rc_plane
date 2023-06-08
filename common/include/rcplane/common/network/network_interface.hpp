#ifndef __RCPLANE__COMMON__NETWORK__NETWORK_INTERFACE_HPP__
#define __RCPLANE__COMMON__NETWORK__NETWORK_INTERFACE_HPP__

#include <functional>

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

  virtual void register_start_handler(std::function<void()> start_handler) {
    _start_handler = start_handler;
  }

  virtual void register_stop_handler(std::function<void()> stop_handler) {
    _stop_handler = stop_handler;
  }

 protected:
  std::function<void()> _stop_handler;
  std::function<void()> _start_handler;
  std::function<void(int)> _termination_handler;
};

} // namespace interface
} // namespace network
} // namespace common
} // namespace rcplane

#endif //__RCPLANE__COMMON__NETWORK__NETWORK_INTERFACE_HPP__

