#ifndef __RCPLANE__COMMON__IO__PACKET_HPP__
#define __RCPLANE__COMMON__IO__PACKET_HPP__

#include "rcplane/common/io/journal.hpp"

namespace rcplane {
namespace common {
namespace io {

enum packet_type {
  state = 0,
  motor = 1,

  // Control surfaces
  aileron = 2,
  elevator = 3,
  rudder = 4,

  // Gyroscope
  pitch = 5,
  roll = 6,
  yaw = 7, 

  timestamp = 100,

  invalid = 255,
};

template <typename BUFFER_SIZE, typename RETURN_TYPE>
class packet {
 public:
  packet() : packet(packet_type::invalid) {
  }

  explicit packet(packet_type type) {
    // Ensure that conversions will be perserved
    static_assert(sizeof(BUFFER_SIZE) <= sizeof(RETURN_TYPE));
    _type = type; 
  }

  ~packet() = default;

  void set(BUFFER_SIZE buffer) {
    _converter.raw = buffer; 
  }

  BUFFER_SIZE raw() {
    return _converter.raw;
  }

  RETURN_TYPE data() {
    return _converter.data;
  }

  packet_type type() {
    return _type;
  }

  std::string type_to_str() {
    switch (_type) {
      case packet_type::state:    return "state";
      case packet_type::motor:    return "motor";
      case packet_type::aileron:  return "aileron";
      case packet_type::elevator: return "elevator";
      case packet_type::rudder:   return "rudder";
      case packet_type::pitch:    return "pitch";
      case packet_type::roll:     return "roll";
      case packet_type::yaw:      return "yaw";
      default:             return "invalid";
    }
    return "";
  }

 private:
  packet_type _type;

  // Trick to fast conversion. Set the buffer here and pull 
  // the data to get the value in it's expected return type
  // since they share the same memory region.
  union conversion {
    BUFFER_SIZE raw;
    RETURN_TYPE data; 
  };
  conversion _converter;
};

} // namesapce io
} // namesapce common
} // namesapce rcplane

#endif //__RCPLANE__COMMON__IO__PACKET_HPP__
