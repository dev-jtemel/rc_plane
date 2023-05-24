#ifndef __MCU__LIB__POWER_LED_HPP__
#define __MCU__LIB__POWER_LED_HPP__

#include <Arduino.h>
#include "../pins.hpp"
#include "../interface/led_controller.hpp"

namespace mcu {
namespace lib {

class power_led : public interface::led_controller {
 public:
  power_led() : interface::led_controller("power_led", 500) {
  }

  ~power_led() = default;

  virtual bool setup() {
    pinMode(pins::led::power::OUT_PIN, OUTPUT);

    off();

    log("setup complete");
  }

  virtual void test() {
  }

  virtual void on() {
    digitalWrite(pins::led::power::OUT_PIN, HIGH); 
    log("on");
  }

  virtual void off() {
    digitalWrite(pins::led::power::OUT_PIN, LOW); 
    log("off");
  }

  virtual void stop() {
    off();
  }
};

} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__POWER_LED_HPP__
