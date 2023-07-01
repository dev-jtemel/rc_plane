#ifndef __RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__
#define __RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__

#include <array>
#include <fstream>
#include <functional>
#include <string>
#include <termios.h>

#include "rcplane/common/base_controller.hpp"
#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/packet.hpp"

#define _POSIX_SOURCE 1

namespace rcplane {
namespace common {
namespace io {

class serial_controller : public ::rcplane::common::interface::base_controller {
public:
  serial_controller();
  ~serial_controller();

  bool init() override;
  void start() override;
  void terminate() override;

  void register_cs_cb(
      std::function<void(uint8_t, uint8_t, int8_t, int8_t, int8_t)> cb);
  void register_gyro_cb(std::function<void(float, float, float)> cb);

private:
  void p_read_serial();
  void p_read_log();

  void p_handle_buffer();

  // Should appear 4 times in a row on a new connection.
  const uint64_t START_INDICATOR = 0xFFFFFFFFFFFFFFFF;
  uint8_t _startcount = 0;
  uint8_t _line = 0;

  static const uint32_t MAX_LEN = 17U;
  const std::string _tty = "/dev/ttyACM0";
  int _fd;
  int _res;
  termios _otio;
  termios _ntio;
  char _buf[MAX_LEN];
  std::ofstream _blackbox;

  uint64_t _buffer;
  packet<uint32_t, uint32_t> _timestamp;
  packet<uint8_t, uint8_t> _state, _motor;
  packet<uint8_t, int8_t> _aileron, _elevator, _rudder;
  packet<uint32_t, float> _pitch, _roll, _yaw;

  std::function<void(uint8_t, uint8_t, int8_t, int8_t, int8_t)> _cs_cb;
  std::function<void(float, float, float)> _gyro_cb;
};

}  // namespace io
}  // namespace common
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__
