#ifndef __RCPLANE__COMMON__IO__PACKET_HPP__
#define __RCPLANE__COMMON__IO__PACKET_HPP__

#include "rcplane/common/io/journal_impl.hpp"

namespace rcplane {
namespace common {
namespace io {

class packet {
 public:
  enum type {
    invalid = 0,
    state,
    motor,
    aileron,
    elevator,
    rudder
  };

  explicit packet(uint32_t buffer) : _buffer(buffer) {
    _type = p_type();
    _data = static_cast<int>(_buffer & DATA) * ((_buffer & SIGN) ? -1 : 1);
  }

  type type() {
    return _type;
  }

  int data() {
    return _data;
  } 

  std::string type_to_str() {
    switch (_type) {
      case type::state:    return "state";
      case type::motor:    return "motor";
      case type::aileron:  return "aileron";
      case type::elevator: return "elevator";
      case type::rudder:   return "rudder";
      default:             return "invalid";
    }
    return "";
  }

 private:
  enum type p_type() {
    if (STATE & _buffer) {
      return type::state;
    } else if (MOTOR & _buffer) {
      return type::motor;
    } else if (AILERON & _buffer) {
      return type::aileron;
    } else if (ELEVATOR & _buffer) {
      return type::elevator;
    } else if (RUDDER & _buffer) {
      return type::rudder;
    } else {
      return type::invalid;
    }
  }

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
