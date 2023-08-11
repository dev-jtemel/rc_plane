#ifndef __RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__
#define __RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__

#include <array>
#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <fstream>
#include <functional>
#include <string>

#include "rcplane/common/base_controller.hpp"
#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/packet.hpp"

namespace rcplane {
namespace common {
namespace io {

class serial_controller : public ::rcplane::common::interface::base_controller {
public:
  serial_controller();
  virtual ~serial_controller();

  bool init() override;
  void start() override;
  void terminate() override;

  void register_cs_cb(
      std::function<void(uint8_t, uint8_t, int8_t, int8_t, int8_t)> cb);
  void register_gyro_cb(std::function<void(float, float, float)> cb);

private:
  virtual void p_read_serial();
  virtual bool p_open_port();
  virtual boost::optional<uint64_t> p_read_line();
  void p_read_log();

  void p_handle_buffer();

  virtual bool p_handshake_mcu();

  uint8_t _line = 0;
  const std::string TTY = "/dev/ttyACM0";
  const std::string HELLO_RX = "1";
  std::ofstream _blackbox;

  boost::asio::streambuf _streambuffer;
  uint64_t _buffer;
  packet<uint32_t, uint32_t> _timestamp;
  packet<uint8_t, uint8_t> _state, _motor;
  packet<uint8_t, int8_t> _aileron, _elevator, _rudder;
  packet<uint32_t, float> _pitch, _roll, _yaw;

  std::function<void(uint8_t, uint8_t, int8_t, int8_t, int8_t)> _cs_cb;
  std::function<void(float, float, float)> _gyro_cb;

  boost::asio::io_service _io;
  boost::asio::serial_port _serial;
};

}  // namespace io
}  // namespace common
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__
