#ifndef __MCU__LIB__INTERFACE__SWITCH_CONTROLLER_HPP__
#define __MCU__LIB__INTERFACE__SWITCH_CONTROLLER_HPP__

#include <Arduino.h>
#include "servo.hpp"

namespace mcu {
namespace lib {
namespace interface {

class switch_controller {
 public:
  switch_controller() = default;

  switch_controller(String TAG_) : TAG(TAG_) {
  }

  ~switch_controller() = default;

  virtual bool setup() {
    return false;
  }

  virtual bool state() {
    return false;
  }

  template <typename T>
  void log(T t) {
    if (_log_open) {
      Serial.print(t);
      return;
    }

    Serial.print("[");
    Serial.print(micros());
    Serial.print("] ");
    Serial.print("[");
    Serial.print(TAG);
    Serial.print("] ");
    Serial.println(t);
  }

  void open_log() {
    Serial.print("[");
    Serial.print(micros());
    Serial.print("] ");
    Serial.print("[");
    Serial.print(TAG);
    Serial.print("] ");
    _log_open = true;
  }

  void close_log() {
    Serial.println();
    _log_open = false;
  }

 protected:
  const double PVM_MID = 1500.00;
  String TAG;
 
 private:
  bool _log_open;
};

} // namespace interface
} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__INTERFACE__SWITCH_CONTROLLER_HPP__
