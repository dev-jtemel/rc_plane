#ifndef __MCU__LIB__INTERFACE__SWITCH_CONTROLLER_HPP__
#define __MCU__LIB__INTERFACE__SWITCH_CONTROLLER_HPP__

#include <Arduino.h>
#include "servo.hpp"

namespace mcu {
namespace lib {
namespace interface {

class switch_controller {
 public:
  switch_controller() = default;

  ~switch_controller() = default;

  virtual bool setup() {
    return false;
  }

  virtual bool state() {
    return false;
  }

 protected:
  const double PVM_MID = 1500.00;
};

} // namespace interface
} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__INTERFACE__SWITCH_CONTROLLER_HPP__
