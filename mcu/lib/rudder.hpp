#ifndef __MCU__LIB__RUDDER_HPP__
#define __MCU__LIB__RUDDER_HPP__

#include <Arduino.h>
#include "controller.hpp"

namespace mcu {
namespace lib {

class rudder : public interface::controller {
 public:
  const uint8_t IN_PIN = 4U;
  const uint8_t OUT_PIN = 11U;

  rudder() : interface::controller("rudder", 115, 35, -35) {
  }

  ~rudder() = default;

  virtual bool setup() {
    _servos = new servo[1];
    if (_servos == nullptr) {
      return false;
    }

    pinMode(IN_PIN, INPUT);
    pinMode(OUT_PIN, OUTPUT);

    _servos[0].bind(OUT_PIN);

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
    _pulse = toRange(pulseIn(IN_PIN, HIGH));
    _servos[0].write(NEUTRAL - _pulse);
  }

};

} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__RUDDER_HPP__
