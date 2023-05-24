#ifndef __RCPLANE__COMMON__IO__SERIAL_HPP__
#define __RCPLANE__COMMON__IO__SERIAL_HPP__

#include <termios.h>

#include <functional>
#include <vector>
#include <string>

#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/packet.hpp"

#define _POSIX_SOURCE 1

namespace rcplane {
namespace common {
namespace io {

class serial {
 public:
  serial();
  ~serial();

  void read_serial();

  void register_cb(std::function<void(std::vector<packet> &)> cb);

 private:
  static const uint32_t MAX_LEN = 33U;
  const std::string TAG = "serial";
  const std::string _tty = "/dev/ttyACM0";
  int _fd;
  int _res;
  termios _otio;
  termios _ntio;
  char _buf[MAX_LEN];

  std::vector<packet> _packets;
  std::function<void(std::vector<packet> &)> _cb;
};

} // namesapce io
} // namesapce common
} // namesapce rcplane

#endif //__RCPLANE__COMMON__IO__SERIAL_HPP__
