#ifndef __RCPLANE__COMMON__STATE_HPP__
#define __RCPLANE__COMMON__STATE_HPP__

#ifndef ARDUINO_ARCH_AVR
#  include <cstdint>
#endif

namespace rcplane {
namespace common {
namespace state {

// If, set user is currently manipulating the roll TX stick.
// Ignored if kASSISTANCE_FLAG is not set.
const uint8_t kUSER_ROLL = 0x01;
// If, set user is currently manipulating the pitch TX stick.
// Ignored if kASSISTANCE_FLAG is not set.
const uint8_t kUSER_PITCH = 0x02;

// If set, apply flight stabilization
const uint8_t kASSISTANCE_FLAG = 0x80;

}  // namespace state
}  // namespace common
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__STATE_HPP__