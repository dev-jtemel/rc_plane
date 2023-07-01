#ifndef __MCU__LIB__TEST_SWTICH_HPP__
#define __MCU__LIB__TEST_SWTICH_HPP__

#include "../interface/switch_controller.hpp"
#include "../pins.hpp"
#include <Arduino.h>

namespace mcu {
namespace lib {

class test_switch : public interface::switch_controller {
public:
  test_switch() : interface::switch_controller() {}

  ~test_switch() = default;

  virtual bool setup() { pinMode(pins::test_switch::IN_PIN, INPUT); }

  virtual void test() {}

  virtual bool state() {
    bool state = pulseIn(pins::test_switch::IN_PIN, HIGH) > PVM_MID;
    return state;
  }
};

}  // namespace lib
}  // namespace mcu

#endif  //__MCU__LIB__TEST_SWTICH_HPP__
