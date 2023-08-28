#ifndef RCPLANE_IO_SERIAL_CONTROLLER_HPP
#define RCPLANE_IO_SERIAL_CONTROLLER_HPP

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <cstdint>
#include <string>
#include <type_traits>

#include "rcplane/common/Packet.hpp"
#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace io {

/**
 * @brief Control communication with a serial device.
 */
class SerialController : public ::boost::noncopyable {
public:
  /**
   * @brief Construct a new Serial Controller object.
   * 
   * @param cm The configuration manager.
   * @param io_service The main IO service.
   */
  SerialController(const ConfigManager &configManager,
                   boost::asio::io_service &io_service);
  ~SerialController();

  /**
   * @brief Open the serial port c_ttyDev with c_baudRate.
   * @return bool True if the serial port is opened and configured successfully.
   */
  bool open();

  /**
   * @brief Struct to indicate status of the readPacket call.
   * @tparam PACKET_TYPE The packet type to read.
   * @tparam std::enable_if_t<
   * std::is_base_of_v<common::BasePacket, PACKET_TYPE>> 
   */
  template<typename PACKET_TYPE,
           typename = std::enable_if_t<
               std::is_base_of_v<common::BasePacket, PACKET_TYPE>>>
  struct ReadResult {
    PACKET_TYPE packet{};
    bool didTimeout{true};
    size_t bytesRead{};
  };

  /**
   * @brief Read a packet from the serial port.
   * @tparam PACKET_TYPE Packet type derived from rcplane::common::BasePacket.
   * @tparam std::enable_if_t<
   * std::is_base_of_v<common::BasePacket, PACKET_TYPE>> 
   * @return ReadResult<PACKET_TYPE> The packet and metadata read from the serial port.
   */
  template<typename PACKET_TYPE,
           typename = std::enable_if_t<
               std::is_base_of_v<common::BasePacket, PACKET_TYPE>>>
  ReadResult<PACKET_TYPE> readPacket();

  /**
   * @brief Write a packet to the serial port.
   * 
   * @tparam PACKET_TYPE Packet type derived from rcplane::common::BasePacket.
   * @tparam std::enable_if<
   * std::is_base_of<common::BasePacket, PACKET_TYPE>::value>::type 
   * @param packet The packet to write to the serial port.
   * @return bool True if the packet was written successfully.
   */
  template<typename PACKET_TYPE,
           typename = typename std::enable_if_t<
               std::is_base_of_v<common::BasePacket, PACKET_TYPE>>>
  bool writePacket(const PACKET_TYPE &packet);

  /**
   * @brief Flush the serial port by reading all incoming bytes until a new
   * line character is read.
   * @return bool If the newline character was found and consumed, false if 
   * read timeouted.
   */
  bool flush();

private:
  std::string c_ttyDev;
  uint32_t c_baudRate;
  uint32_t c_readTimeoutMs;
  uint32_t c_writeTimeoutMs;
  std::string c_handshakeTerminationStr;

  boost::asio::streambuf m_streamBuffer;
  boost::asio::serial_port m_serialPort;
  boost::asio::io_service &m_ioService;
};

}  // namespace io
}  // namespace rcplane

#endif  // RCPLANE_IO_SERIAL_CONTROLLER_HPP