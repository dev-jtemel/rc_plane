#ifndef RCPLANE_COMMON_PACKET_HPP
#define RCPLANE_COMMON_PACKET_HPP

#ifndef ARDUINO_ARCH_AVR
#  include <cstdint>
#endif

namespace rcplane {
namespace common {

/**
 * @brief Used to represent a packet type.
 */
struct BasePacket {};

struct __attribute((packed)) HandshakePacket : public BasePacket {
  uint8_t handshake{};

  bool operator==(const HandshakePacket &other) const {
    return handshake == other.handshake;
  }

  bool operator!=(const HandshakePacket &other) const {
    return handshake != other.handshake;
  }
};

/**
 * @brief Packet definition of time and state of the application. 
 * @warning This packet is packed to avoid compiler padding.
 */
struct __attribute((packed)) StatePacket : public BasePacket {
  uint32_t timestamp{};
  uint8_t state{};

  bool operator==(const StatePacket &other) const {
    return timestamp == other.timestamp && state == other.state;
  }

  bool operator!=(const StatePacket &other) const { return !(*this == other); }
};

/**
 * @brief Packet definition of control surface values.
 * 
 * This packet defines the control surface data to be communicated
 * to and from the mcu and som.
 * 
 * @warning This packet is packed to avoid compiler padding.
 */
struct __attribute__((packed)) ControlSurfacePacket : public BasePacket {
  uint8_t motorSpeed{};
  int8_t aileronDeflection{};
  int8_t elevatorDeflection{};
  int8_t rudderDeflection{};

  bool operator==(const ControlSurfacePacket &other) const {
    return motorSpeed == other.motorSpeed
        && aileronDeflection == other.aileronDeflection
        && elevatorDeflection == other.elevatorDeflection
        && rudderDeflection == other.rudderDeflection;
  }

  bool operator!=(const ControlSurfacePacket &other) const {
    return !(*this == other);
  }
};

/**
 * @brief Packet definition of gyro scope readings.
 * 
 * This packet defines the (formed) gyro and accelerometer data to be communicated
 * to and from the mcu and som.
 * 
 * @warning This packet is packed to avoid compiler padding.
 */
struct __attribute__((packed)) ImuPacket : public BasePacket {
  float gyroX{};
  float gyroY{};
  float gyroZ{};
  float accX{};
  float accY{};
  float accZ{};
  float temperature{};

  bool operator==(const ImuPacket &other) const {
    return gyroX == other.gyroX && gyroY == other.gyroY && gyroZ == other.gyroZ
        && accX == other.accX && accY == other.accY && accZ == other.accZ
        && temperature == other.temperature;
  }

  bool operator!=(const ImuPacket &other) const { return !(*this == other); }
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

/**
 * @brief Read the packet from UART.
 * @tparam PACKET The packet struct to read from the serial console.
 * @param packet The packet (lvalue) to read into.
 */
template<typename PACKET>
size_t read_packet(PACKET &packet) {
  return Serial.readBytes((uint8_t *)&packet, sizeof(packet));
}
#endif

}  // namespace common
}  // namespace rcplane

#endif  //RCPLANE_COMMON_PACKET_HPP