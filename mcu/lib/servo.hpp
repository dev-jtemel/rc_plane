#ifndef __MCU__LIB__SERVO_HPP__
#define __MCU__LIB__SERVO_HPP__

#include <Arduino.h>

namespace mcu {
namespace lib {

class servo {
 public:
  servo() = default;
  ~servo() = default;

  void bind(uint8_t pin) {
    _pin = pin;
  }

  void write(int value) {
    analogWrite(_pin, value);
  }

 private:
  uint8_t _pin;
};

} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__SERVO_HPP__
