#ifndef __MCU__LIB__RUDDER_HPP__
#define __MCU__LIB__RUDDER_HPP__

#include <Arduino.h>
#include "../pins.hpp"
#include "../interface/controller.hpp"

namespace mcu {
namespace lib {

class rudder : public interface::controller {
 public:
  rudder() : interface::controller("rudder", 115, 35, -35) {
  }

  ~rudder() = default;

  virtual bool setup() {
    _servos = new interface::servo[1];
    if (_servos == nullptr) {
      return false;
    }

    pinMode(pins::rudder::IN_PIN, INPUT);
    pinMode(pins::rudder::OUT_PIN, OUTPUT);

    _servos[0].bind(pins::rudder::OUT_PIN);

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
    _pulse = toRange(pulseIn(pins::rudder::IN_PIN, HIGH));
    _servos[0].write(NEUTRAL - _pulse);

    open_log();
    log(NEUTRAL - _pulse);
    close_log();
  }

  virtual void stop() {
    _servos[0].write(NEUTRAL);
  }

};

} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__RUDDER_HPP__
