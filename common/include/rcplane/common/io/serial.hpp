#ifndef __RCPLANE__COMMON__IO__SERIAL_HPP__
#define __RCPLANE__COMMON__IO__SERIAL_HPP__

#include <termios.h>

#include <functional>
#include <fstream>
#include <array>
#include <string>

#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/packet.hpp"
#include "rcplane/common/base_controller.hpp"

#define _POSIX_SOURCE 1

namespace rcplane {
namespace common {
namespace io {

class serial : public ::rcplane::common::interface::base_controller {
 public:
  serial();
  ~serial();

  bool init() override;
  void start() override;
  void terminate() override;

  void register_cb(std::function<void(uint32_t timestamp, std::array<packet, 5U> &)> cb);

 private:
  void p_read_serial();
  void p_read_log();

  void p_handle_buffer();

#ifdef SIMULATION
  std::ifstream _log = std::ifstream("./logs/test.log");
#else
  static const uint32_t MAX_LEN = 17U;
  const std::string _tty = "/dev/ttyACM0";
  int _fd;
  int _res;
  termios _otio;
  termios _ntio;
  char _buf[MAX_LEN];
  std::ofstream _blackbox;
#endif

  uint64_t _buffer;
  std::array<packet, 5U> _packets;
  std::function<void(uint32_t timestamp, std::array<packet, 5U> &)> _cb;
};

} // namesapce io
} // namesapce common
} // namesapce rcplane

#endif //__RCPLANE__COMMON__IO__SERIAL_HPP__
