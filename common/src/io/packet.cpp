#include "rcplane/common/io/packet.hpp"

namespace rcplane {
namespace common {
namespace io {

packet::packet() : _type(type::invalid), _data(0U), _buffer(0U) {
}
packet::packet(uint32_t buffer) : _buffer(buffer) {
  _type = p_type();
  _data = static_cast<int>(_buffer & DATA) * ((_buffer & SIGN) ? -1 : 1);
}

enum packet::type packet::type() {
  return _type;
}

int packet::data() {
  return _data;
} 

std::string packet::type_to_str() {
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

enum packet::type packet::p_type() {
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

} // namesapce io
} // namesapce common
} // namesapce rcplane
