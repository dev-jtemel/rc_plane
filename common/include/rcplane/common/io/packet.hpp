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

  packet();
  explicit packet(enum type type, uint8_t buffer);

  type type();
  int data();

  std::string type_to_str();
 private:
  uint8_t NEGATIVE = 0x8;
  void convert_buffer();
  bool is_twos_compliment();

  enum type _type;
  int _data; 
  uint8_t _buffer;
};

} // namesapce io
} // namesapce common
} // namesapce rcplane

#endif //__RCPLANE__COMMON__IO__PACKET_HPP__
