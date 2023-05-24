#ifndef __MCU__LIB__ELEVATOR_HPP__
#define __MCU__LIB__ELEVATOR_HPP__

#include <Arduino.h>
#include "../pins.hpp"
#include "../interface/controller.hpp"

namespace mcu {
namespace lib {

class elevator : public interface::controller {
 public:
  elevator() : interface::controller("elevator", 115, 50, -50, 0x8000000) {
  }

  ~elevator() = default;

  virtual bool setup() {
    _servos = new interface::servo[1];
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

    // restrict lift to avoid nose dives
    if (_pulse > 0) {
      _pulse /= 2;
    }
    _servos[0].write(NEUTRAL - _pulse);

    serial_log(_pulse);
  }

  virtual void stop() {
    _servos[0].write(NEUTRAL);
  }

};

} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__ELEVATOR_HPP__
