#ifndef RCPLANE_IO_SERIAL_CONTROLLER_HPP
#define RCPLANE_IO_SERIAL_CONTROLLER_HPP

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <cstdint>
#include <string>
#include <type_traits>

#include "rcplane/common/Packet.hpp"
#include "rcplane/io/ConfigManager.hpp"

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
  SerialController(ConfigManager &cm, boost::asio::io_service &io_service);
  ~SerialController();

  /**
   * @brief Open the serial port c_ttyDev with c_baudRate.
   * @return bool True if the serial port is opened and configured successfully.
   */
  bool open();

  /**
   * @brief Read a packet from the serial port.
   * @tparam PACKET_TYPE Packet type derived from rcplane::common::BasePacket.
   * @tparam std::enable_if_t<
   * std::is_base_of_v<common::BasePacket, PACKET_TYPE>> 
   * @return PACKET_TYPE The packet read from the serial port (0 initialized if timeouted).
   */
  template<typename PACKET_TYPE,
           typename = std::enable_if_t<
               std::is_base_of_v<common::BasePacket, PACKET_TYPE>>>
  PACKET_TYPE readPacket();

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

private:
  std::string c_ttyDev;
  uint32_t c_baudRate;
  uint32_t c_readTimeoutMs;
  uint32_t c_writeTimeoutMs;

  boost::asio::streambuf m_streamBuffer;
  boost::asio::serial_port m_serialPort;
  boost::asio::io_service &m_ioService;
};

}  // namespace io
}  // namespace rcplane

#endif  // RCPLANE_IO_SERIAL_CONTROLLER_HPP