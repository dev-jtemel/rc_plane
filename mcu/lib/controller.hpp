#ifndef __MCU__LIB__CONTROLLER_HPP__
#define __MCU__LIB__CONTROLLER_HPP__

#include <Arduino.h>
#include "servo.hpp"

namespace mcu {
namespace lib {
namespace interface {

class controller {
 public:
  controller() = default;
  ~controller() = default;

  virtual bool setup() = 0;
  virtual void test() = 0;
  virtual void step() = 0;

 protected:
  uint8_t *_ins;
  uint8_t *_outs;
  servo *_servos;
};

} // namespace interface
} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__CONTROLLER_HPP__
