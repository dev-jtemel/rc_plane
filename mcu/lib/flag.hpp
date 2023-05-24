#ifndef __MCU__LIB__FLAG_HPP__
#define __MCU__LIB__FLAG_HPP__

#include <Arduino.h>

namespace mcu {
namespace lib {
namespace flag {

const uint16_t FLIGHT_MODE = 0x1;

const uint16_t TEST_FLAGS[] = {
0x2,
0x4,
0x8,
0x10,
0x0,
0x0
};

const uint16_t TEST_COMPLETE = 0x1E;

} // namespace flag
} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__FLAG_HPP__
