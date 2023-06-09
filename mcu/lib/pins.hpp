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

}  // namespace motor

namespace aileron {

const uint8_t IN_PIN = 2U;
const uint8_t OUT_PIN = 8U;

}  // namespace aileron

namespace elevator {

const uint8_t IN_PIN = 3U;
const uint8_t OUT_PIN = 10U;

}  // namespace elevator

namespace rudder {

const uint8_t IN_PIN = 4U;
const uint8_t OUT_PIN = 11U;

}  // namespace rudder

namespace led {

namespace wing {

const uint8_t OUT_PIN = 23U;

}  // namespace wing

namespace power {

const uint8_t OUT_PIN = 25U;

}  // namespace power

}  // namespace led

namespace test_switch {

const uint8_t IN_PIN = 52U;

}  // namespace test_switch

namespace flight_switch {

const uint8_t IN_PIN = 53U;

}  // namespace flight_switch

}  // namespace pins
}  // namespace lib
}  // namespace mcu

#endif  //__MCU__LIB__PINS_HPP__
