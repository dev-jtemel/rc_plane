#ifndef __MCU__LIB__PINS_HPP__
#define __MCU__LIB__PINS_HPP__

#include <Arduino.h>

namespace mcu {
namespace lib {
namespace pins {

namespace motor {

const uint8_t IN_PIN = 5;
const uint8_t OUT_1_PIN = 22;
const uint8_t OUT_2_PIN = 24;
const uint8_t ENABLE_PIN = 12;

} // namespace motor


namespace aileron {

const uint8_t IN_PIN = 2U;
const uint8_t LEFT_PIN = 8U;
const uint8_t RIGHT_PIN = 9U;

} // namespace aileron

namespace elevator {

const uint8_t IN_PIN = 3U;
const uint8_t OUT_PIN = 10U;

} // namespace elevator

namespace rudder {

const uint8_t IN_PIN = 4U;
const uint8_t OUT_PIN = 11U;

} // namespace rudder

} // namespace pins
} // namespace lib
} // namespace mcu
  //
#endif //__MCU__LIB__PINS_HPP__
