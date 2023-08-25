#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <cstdint>
#include <string>
#include <type_traits>

#include "rcplane/common/Packet.hpp"
#include "rcplane/io/ConfigManager.hpp"

namespace rcplane {
namespace io {

class SerialController : public ::boost::noncopyable {
public:
  SerialController(ConfigManager &cm, boost::asio::io_service &io_service);
  ~SerialController();

  bool open();
  bool flushBuffer(const std::string &);

  template<typename PACKET_TYPE,
           typename = std::enable_if_t<
               std::is_base_of_v<common::BasePacket, PACKET_TYPE>>>

  PACKET_TYPE readPacket();

  template<typename PACKET_TYPE,
           typename = typename std::enable_if<
               std::is_base_of<common::BasePacket, PACKET_TYPE>::value>::type>
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