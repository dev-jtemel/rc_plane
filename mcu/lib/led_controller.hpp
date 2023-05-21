#ifndef __MCU__LIB__LED_CONTROLLER_HPP__
#define __MCU__LIB__LED_CONTROLLER_HPP__

#include <Arduino.h>
#include "controller.hpp"

namespace mcu {
namespace lib {
namespace interface {

class led_controller : public controller {
 public:
  led_controller() = default;

  led_controller(String tag, uint64_t interval)
    : interface::controller(tag, 0, 0, 0),
      _on(true),
      _interval(interval),
      _prev_ms(millis()) {
  }

  ~led_controller() = default;

  virtual void on() {
  }

  virtual void off() {
  }

  virtual void step() {
    uint64_t ms = millis();
    if (ms - _prev_ms >= _interval) {
      _prev_ms = ms;

      if (_on) {
        on();
      } else {
        off();
      }
      _on = !_on;
    }
  }

 private:
  bool _on;
  uint64_t _prev_ms;
  uint64_t _interval;
};

} // namespace interface
} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__LED_CONTROLLER_HPP__
