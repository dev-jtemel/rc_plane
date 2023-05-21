#ifndef __MCU__LIB__MOTOR_HPP__
#define __MCU__LIB__MOTOR_HPP__

#include <Arduino.h>
#include "pins.hpp"
#include "controller.hpp"

namespace mcu {
namespace lib {

class motor : public interface::controller {
 public:
  motor() : interface::controller("motor", 0, 255, 0) {
  }

  ~motor() = default;

  virtual bool setup() {
    pinMode(pins::motor::IN_PIN, INPUT);
    pinMode(pins::motor::OUT_1_PIN, OUTPUT);
    pinMode(pins::motor::OUT_2_PIN, OUTPUT);
    pinMode(pins::motor::ENABLE_PIN, OUTPUT);

    analogWrite(pins::motor::ENABLE_PIN, NEUTRAL);
    digitalWrite(pins::motor::OUT_1_PIN, HIGH);
    digitalWrite(pins::motor::OUT_2_PIN, LOW);

    log("setup complete");
  }

  virtual void test() {
    for (double i = 0; i <= MAX_OFFSET/3; ++i) {
      analogWrite(pins::motor::ENABLE_PIN, i);
      delay(10);
    }

    delay(1000);

    for (double i = MAX_OFFSET/3; i >= 0; --i) {
      analogWrite(pins::motor::ENABLE_PIN, i);
      delay(10);
    }

    log("test complete");
  }

  virtual void step() {
    _pulse = toRange(pulseIn(pins::motor::IN_PIN, HIGH));

    // restrict lift to avoid nose dives
    if (_pulse > MAX_OFFSET - 10) {
      _pulse = MAX_OFFSET;
    }
    analogWrite(pins::motor::ENABLE_PIN, _pulse);

    open_log();
    log(_pulse);
    close_log();
  }

};

} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__MOTOR_HPP__
