#ifndef __MCU__LIB__TEST_SWTICH_HPP__
#define __MCU__LIB__TEST_SWTICH_HPP__

#include <Arduino.h>
#include "../pins.hpp"
#include "../interface/switch_controller.hpp"

namespace mcu {
namespace lib {

class test_switch : public interface::switch_controller{
 public:
  test_switch() : interface::switch_controller("test_switch") {
  }

  ~test_switch() = default;

  virtual bool setup() {
    pinMode(pins::test_switch::IN_PIN, INPUT);

    log("setup complete");
  }

  virtual void test() {
  }

  virtual bool state() {
    bool state = pulseIn(pins::test_switch::IN_PIN, HIGH) > PVM_MID;
    open_log();
    log(state);
    close_log();
    return state;
  }

};

} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__TEST_SWTICH_HPP__
