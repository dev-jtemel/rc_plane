#ifndef __MCU__LIB__CONTROLLER_HPP__
#define __MCU__LIB__CONTROLLER_HPP__

#include <Arduino.h>
#include "servo.hpp"

namespace mcu {
namespace lib {
namespace interface {

class controller {
 public:
  controller() = default;

  controller(String TAG_, double NEUTRAL_, double MAX_OFFSET_, double MIN_OFFSET_) 
    : TAG(TAG_), NEUTRAL(NEUTRAL_), MAX_OFFSET(MAX_OFFSET_), MIN_OFFSET(MIN_OFFSET_) {
  }

  ~controller() {
    if (_servos != nullptr) {
      delete[] _servos;
    }
  }

  virtual bool setup() {
    return false;
  }

  virtual void test() {
  }

  virtual void step() {
  }

  double toRange(double value) {
    return ((value - PVM_MIN) / (PVM_MAX - PVM_MIN)) * (MAX_OFFSET - MIN_OFFSET) + MIN_OFFSET; 
  }

  template <typename T>
  void log(T t) {
    Serial.print("[");
    Serial.print(micros());
    Serial.print("] ");
    Serial.print("[");
    Serial.print(TAG);
    Serial.print("] ");
    Serial.println(t);
  }

 protected:
  servo *_servos;
  double _pulse;

  String TAG;
  double NEUTRAL;
  double MIN_OFFSET;
  double MAX_OFFSET;

 private:
  const double PVM_MIN = 1000.00;
  const double PVM_MAX = 2000.00;

};

} // namespace interface
} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__CONTROLLER_HPP__
