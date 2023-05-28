#include "rcplane/common/io/packet.hpp"

namespace rcplane {
namespace common {
namespace io {

packet::packet() : _type(type::invalid), _data(0), _buffer(0U) {
}

packet::packet(enum type type, uint8_t buffer) : _type(type), _buffer(buffer) {
  convert_buffer();
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

void packet::convert_buffer() {
  if (is_twos_compliment() && (NEGATIVE & _buffer)) {
    _data = -static_cast<int>(static_cast<uint8_t>(~_buffer) + 1U);
  } else {
    _data = static_cast<int>(_buffer);
  }
}

bool packet::is_twos_compliment() {
  switch (_type) {
    case type::aileron:
    case type::elevator:
    case type::rudder:
      return true;
    case type::state:
    case type::motor:
    default:
      return false;
  }
  return false;
}

} // namesapce io
} // namesapce common
} // namesapce rcplane
