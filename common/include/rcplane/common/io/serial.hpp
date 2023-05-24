#ifndef __RCPLANE__COMMON__IO__SERIAL_HPP__
#define __RCPLANE__COMMON__IO__SERIAL_HPP__

#include <strings.h>
#include <string.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>

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

 private:
  const std::string TAG = "serial";
  static const uint32_t MAX_LEN = 33U;
  const std::string _tty = "/dev/ttyACM0";
  int _fd;
  int _res;
  struct termios _otio;
  struct termios _ntio;
  char _buf[MAX_LEN];
};

} // namesapce io
} // namesapce common
} // namesapce rcplane

#endif //__RCPLANE__COMMON__IO__SERIAL_HPP__
