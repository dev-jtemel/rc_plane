#ifndef __RCPLANE__COMMON__IO__PACKET_HPP__
#define __RCPLANE__COMMON__IO__PACKET_HPP__

#include "rcplane/common/io/journal.hpp"

namespace rcplane {
namespace common {
namespace io {

/**
 * @brief Available packet types supported.
 */
enum class packet_type : uint8_t {
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

/**
 * @brief Construct a packet to convert serial data into desired type.
 */
template<typename BUFFER_SIZE, typename RETURN_TYPE>
class packet {
public:
  packet() : packet(packet_type::invalid) {}

  /**
   * @param type The packet type.
   * @warning RETURN_TYPE must be more or equal bytes to BUFFER_SIZE.
   */
  explicit packet(packet_type type) {
    // Ensure that conversions will be perserved
    static_assert(sizeof(BUFFER_SIZE) <= sizeof(RETURN_TYPE));
    _type = type;
  }

  ~packet() = default;

  /**
   * @brief Set the raw buffer data.
   * @param buffer The buffer to set and later convert.
   */
  void set(BUFFER_SIZE buffer) { _converter.raw = buffer; }

  /**
   * @return The raw buffer contents.
   */
  BUFFER_SIZE raw() { return _converter.raw; }

  /**
   * @return The converted buffer contents.
   */
  RETURN_TYPE data() { return _converter.data; }

  /**
   * @return The type of packet.
   */
  packet_type type() { return _type; }

  /**
   * @brief Helper to convert the packet type to a string.
   */
  std::string type_to_str() {
    switch (_type) {
      case packet_type::state: return "state";
      case packet_type::motor: return "motor";
      case packet_type::aileron: return "aileron";
      case packet_type::elevator: return "elevator";
      case packet_type::rudder: return "rudder";
      case packet_type::pitch: return "pitch";
      case packet_type::roll: return "roll";
      case packet_type::yaw: return "yaw";
      default: return "invalid";
    }
    return "";
  }

private:
  packet_type _type;

  /**
   * @brief Conversion union.
   *
   * Trick to fast conversion. Set the buffer here and pull
   * the data to get the value in it's expected return type
   * since they share the same memory region.
   */
  union conversion {
    BUFFER_SIZE raw;
    RETURN_TYPE data;
  };
  conversion _converter;
};

}  // namespace io
}  // namespace common
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__IO__PACKET_HPP__
