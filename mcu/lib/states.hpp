#ifndef __MCU__LIB__FSM__STATES_HPP__
#define __MCU__LIB__FSM__STATES_HPP__

#include <Arduino.h>

namespace mcu {
namespace lib {
namespace fsm {

namespace flag {
  const uint8_t FLIGHT_MODE = 0x1;
  const uint8_t TEST_COMPLETE = 0x2;
  const uint8_t RX_CONNECTED = 0x4;
} // namespace flag

} // namespace fsm
} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__FSM__STATES_HPP__
