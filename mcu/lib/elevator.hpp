#ifndef __MCU__LIB__ELEVATOR_HPP__
#define __MCU__LIB__ELEVATOR_HPP__

#include <Arduino.h>
#include "pins.hpp"
#include "controller.hpp"

namespace mcu {
namespace lib {

class elevator : public interface::controller {
 public:
  elevator() : interface::controller("elevator", 115, 35, -35) {
  }

  ~elevator() = default;

  virtual bool setup() {
    _servos = new servo[1];
    if (_servos == nullptr) {
      return false;
    }

    pinMode(pins::elevator::IN_PIN, INPUT);
    pinMode(pins::elevator::OUT_PIN, OUTPUT);

    _servos[0].bind(pins::elevator::OUT_PIN);

    log("setup complete");
  }

  virtual void test() {
    _servos[0].write(NEUTRAL);
    delay(1000);

    _servos[0].write(NEUTRAL - MAX_OFFSET);
    delay(1000);

    _servos[0].write(NEUTRAL + MAX_OFFSET);
    delay(1000);

    _servos[0].write(NEUTRAL);
    delay(1000);

    log("test complete");
  }

  virtual void step() {
    _pulse = toRange(pulseIn(pins::elevator::IN_PIN, HIGH));
    _servos[0].write(NEUTRAL - _pulse);
  }

};

} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__ELEVATOR_HPP__
