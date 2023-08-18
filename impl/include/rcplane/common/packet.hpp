#ifndef __RCPLANE__COMMON__PACKET_HPP__
#define __RCPLANE__COMMON__PACKET_HPP__

#ifndef ARDUINO_ARCH_AVR
#  include "rcplane/io/journal.hpp"
#endif

namespace rcplane {
namespace common {

/**
 * @brief Packet definition of control surface values.
 * 
 * This packet defines the control surface data to be communicated
 * to and from the mcu and som.
*/
struct __attribute__((packed)) control_surface_packet {
  uint32_t timestamp;
  uint8_t state;
  uint8_t motor;
  int8_t aileron;
  int8_t elevator;
  int8_t rudder;
};

#ifdef ARDUINO_ARCH_AVR
/**
 * @brief Write the packet to UART.
 * @tparam PACKET The packet struct to write to the serial console.
 * @param packet The packet (lvalue) to write.
*/
template<typename PACKET>
void write_packet(PACKET &packet) {
  Serial.write((byte *)&packet, sizeof(packet));
}
#endif

}  // namespace common
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__PACKET_HPP__