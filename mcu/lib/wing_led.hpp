#ifndef __MCU__LIB__WING_LED_HPP__
#define __MCU__LIB__WING_LED_HPP__

#include <Arduino.h>
#include "pins.hpp"
#include "led_controller.hpp"

namespace mcu {
namespace lib {

class wing_led : public interface::led_controller {
 public:
  wing_led() : interface::led_controller("wing_leds", 1000) {
  }

  ~wing_led() = default;

  virtual bool setup() {
    pinMode(pins::led::wing::OUT_PIN, OUTPUT);

    off();

    log("setup complete");
  }

  virtual void test() {
  }

  virtual void on() {
    digitalWrite(pins::led::wing::OUT_PIN, HIGH); 
    log("on");
  }

  virtual void off() {
    digitalWrite(pins::led::wing::OUT_PIN, LOW); 
    log("off");
  }
};

} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__WING_LED_HPP__
