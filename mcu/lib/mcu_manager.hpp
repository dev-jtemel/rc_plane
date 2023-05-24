#ifndef __MCU__LIB__MCU_MANAGER_HPP__
#define __MCU__LIB__MCU_MANAGER_HPP__

#include <Arduino.h>
#include "./controllers/aileron.hpp"
#include "./controllers/elevator.hpp"
#include "./controllers/rudder.hpp"
#include "./controllers/motor.hpp"
#include "./leds/wing_led.hpp"

namespace mcu {
namespace lib {
 
class mcu_manager {
 public:
  mcu_manager() = default;
  ~mcu_manager() = default;

  void init() {
    _controllers[0] = &_motor;
    _controllers[1] = &_aileron;
    _controllers[2] = &_elevator;
    _controllers[3] = &_rudder;
    _controllers[4] = &_wing_led;
  }

  bool setup() {
    for (auto ctr : _controllers) {
      if (!ctr->setup()) {
        return false;
      }
    }
    return true;
  }

  void test() {
    for (auto ctr : _controllers) {
      ctr->test();
    }
  }

  void step() {
    for (auto ctr : _controllers) {
      ctr->step();
    }
  }

 private:
  static const uint8_t CONTROLLERS_COUNT = 5U;

  motor _motor;
  aileron _aileron;
  elevator _elevator;
  rudder _rudder;
  wing_led _wing_led;

  interface::controller *_controllers[CONTROLLERS_COUNT];
};

} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__MCU_MANAGER_HPP__
