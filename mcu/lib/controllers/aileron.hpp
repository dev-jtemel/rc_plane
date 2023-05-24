#ifndef __MCU__LIB__AILERON_HPP__
#define __MCU__LIB__AILERON_HPP__

#include <Arduino.h>
#include "../pins.hpp"
#include "../interface/controller.hpp"

namespace mcu {
namespace lib {

class aileron : public interface::controller {
 public:
  const uint8_t LEFT = 0U;
  const uint8_t RIGHT = 1U;

  aileron() : interface::controller("aileron", 115, 30, -30, 0x4000000) {
  }

  ~aileron() = default;

  virtual bool setup() {
    _servos = new interface::servo[2];
    if (_servos == nullptr) {
      return false;
    }

    pinMode(pins::aileron::IN_PIN, INPUT);
    pinMode(pins::aileron::LEFT_PIN, OUTPUT);
    pinMode(pins::aileron::RIGHT_PIN, OUTPUT);

    _servos[LEFT].bind(pins::aileron::LEFT_PIN);
    _servos[RIGHT].bind(pins::aileron::RIGHT_PIN);
  }

  virtual void test() {
    _servos[LEFT].write(NEUTRAL);
    _servos[RIGHT].write(NEUTRAL);
    delay(200);

    _servos[LEFT].write(NEUTRAL - MAX_OFFSET);
    _servos[RIGHT].write(NEUTRAL + MAX_OFFSET);
    delay(200);

    _servos[LEFT].write(NEUTRAL + MAX_OFFSET);
    _servos[RIGHT].write(NEUTRAL - MAX_OFFSET);
    delay(200);

    _servos[LEFT].write(NEUTRAL);
    _servos[RIGHT].write(NEUTRAL);
    delay(200);
  }

  virtual void step() {
    _pulse = toRange(pulseIn(pins::aileron::IN_PIN, HIGH));
    _servos[LEFT].write(NEUTRAL - _pulse);
    _servos[RIGHT].write(NEUTRAL + _pulse);

    serial_log(_pulse);
  }

  virtual void stop() {
    _servos[LEFT].write(NEUTRAL);
    _servos[RIGHT].write(NEUTRAL);
  }

};

} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__AILERON_HPP__
