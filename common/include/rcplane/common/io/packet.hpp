#ifndef __RCPLANE__COMMON__IO__PACKET_HPP__
#define __RCPLANE__COMMON__IO__PACKET_HPP__

#include "rcplane/common/io/journal.hpp"

namespace rcplane {
namespace common {
namespace io {

class packet {
 public:
  enum type {
    state = 0,
    motor = 1,
    aileron = 2,
    elevator = 3,
    rudder = 4,
    invalid = 255,
  };

  explicit packet(uint32_t buffer);

  type type();
  int data();

  std::string type_to_str();

 private:
  enum type p_type();

  // Bit 31
  const uint32_t SIGN = 0x80000000;

  // Bits 30 - 24
  const uint32_t STATE = 0x1000000;
  const uint32_t MOTOR = 0x2000000;
  const uint32_t AILERON = 0x4000000;
  const uint32_t ELEVATOR = 0x8000000;
  const uint32_t RUDDER = 0x10000000;

  // Bits 23 - 0
  const uint32_t DATA = 0x00FFFFFF;

  enum type _type;
  int _data; 
  uint32_t _buffer;
};

} // namesapce io
} // namesapce common
} // namesapce rcplane

#endif //__RCPLANE__COMMON__IO__PACKET_HPP__
