#ifndef __MCU__LIB__AILERON_HPP__
#define __MCU__LIB__AILERON_HPP__

#include "../interface/controller.hpp"
#include "../pins.hpp"
#include <Arduino.h>

namespace mcu {
namespace lib {

class aileron : public interface::controller {
public:
  aileron() : interface::controller(115, 30, -30) {}

  ~aileron() = default;

  virtual bool setup() {
    _servos = new interface::servo[1];
    if (_servos == nullptr) { return false; }

    pinMode(pins::aileron::IN_PIN, INPUT);
    pinMode(pins::aileron::OUT_PIN, OUTPUT);

    _servos[0].bind(pins::aileron::OUT_PIN);
  }

  virtual void test() {
    _servos[0].write(NEUTRAL);
    delay(200);

    _servos[0].write(NEUTRAL - MAX_OFFSET);
    delay(200);

    _servos[0].write(NEUTRAL + MAX_OFFSET);
    delay(200);

    _servos[0].write(NEUTRAL);
    delay(200);
  }

  virtual void step() {
    _pulse = toRange(pulseIn(pins::aileron::IN_PIN, HIGH));
    _servos[0].write(NEUTRAL - _pulse);
    _state = static_cast<uint8_t>(_pulse);
  }

  virtual void stop() {
    _servos[0].write(NEUTRAL);
    //_servos[RIGHT].write(NEUTRAL);
  }
};

}  // namespace lib
}  // namespace mcu

#endif  //__MCU__LIB__AILERON_HPP__
