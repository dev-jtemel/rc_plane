#ifndef __MCU__LIB__MCU_MANAGER_HPP__
#define __MCU__LIB__MCU_MANAGER_HPP__

#include <Arduino.h>
#include "./controllers/aileron.hpp"
#include "./controllers/elevator.hpp"
#include "./controllers/rudder.hpp"
#include "./controllers/motor.hpp"

#include "./leds/wing_led.hpp"

#include "./controllers/test_switch.hpp"
#include "./controllers/flight_switch.hpp"

#include "flag.hpp"

namespace mcu {
namespace lib {
 
class mcu_manager {
 public:
  mcu_manager() = default;
  ~mcu_manager() = default;

  void init() {
    STATE = 0U;
    _controllers[0] = &_motor;
    _controllers[1] = &_aileron;
    _controllers[2] = &_elevator;
    _controllers[3] = &_rudder;
    _controllers[4] = &_wing_led;
  }

  bool setup() {
    _test_switch.setup();
    _flight_switch.setup();

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
    if (!(STATE & flag::TEST_COMPLETE)) {
      bool state = _test_switch.state();
      if (state) {
        //test();
        STATE |= flag::TEST_COMPLETE;
      }
      return;
    }

    if (_flight_switch.state()) {
      STATE |= flag::FLIGHT_MODE;
    } else {
      STATE &= ~flag::FLIGHT_MODE;
    }

    if (STATE & flag::FLIGHT_MODE) {
      for (auto ctr : _controllers) {
        ctr->step();
      }
    }
  }

 private:
  static const uint8_t CONTROLLERS_COUNT = 5U;
  uint8_t STATE;

  motor _motor;
  aileron _aileron;
  elevator _elevator;
  rudder _rudder;
  wing_led _wing_led;

  interface::controller *_controllers[CONTROLLERS_COUNT];
  test_switch _test_switch;
  flight_switch _flight_switch;
};

} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__MCU_MANAGER_HPP__
