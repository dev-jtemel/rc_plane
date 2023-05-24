#ifndef __MCU__LIB__MCU_MANAGER_HPP__
#define __MCU__LIB__MCU_MANAGER_HPP__

#include <Arduino.h>
#include "./controllers/aileron.hpp"
#include "./controllers/elevator.hpp"
#include "./controllers/rudder.hpp"
#include "./controllers/motor.hpp"

#include "./leds/wing_led.hpp"
#include "./leds/power_led.hpp"

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
    _controllers[5] = &_power_led;
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
    uint16_t i = 0;
    Serial.println(0x1000000 | STATE, BIN);
    for (auto ctr : _controllers) {
      ctr->test();
      STATE |= flag::TEST_FLAGS[i++];
      Serial.println(0x1000000 | STATE, BIN);
    }
  }

  void step() {
    Serial.println(0x1000000 | STATE, BIN);
    if (!(STATE & flag::TEST_COMPLETE)) {
      bool state = _test_switch.state();
      _power_led.on();
      if (state) {
        test();
      }
      return;
    }

    if (_flight_switch.state()) {
      STATE |= flag::FLIGHT_MODE;
      handle_step();
    } else {
      STATE &= ~flag::FLIGHT_MODE;
      stop();
      delay(500);
    }
  }

 private:
  void stop() {
    for (auto ctr : _controllers) {
      ctr->stop();
    }
  }

  void handle_step() {
    for (auto ctr : _controllers) {
      ctr->step();
    }
  }

  static const uint8_t CONTROLLERS_COUNT = 6U;
  uint16_t STATE;

  motor _motor;
  aileron _aileron;
  elevator _elevator;
  rudder _rudder;
  wing_led _wing_led;
  power_led _power_led;

  interface::controller *_controllers[CONTROLLERS_COUNT];
  test_switch _test_switch;
  flight_switch _flight_switch;
};

} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__MCU_MANAGER_HPP__
