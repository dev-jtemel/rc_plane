#include <strings.h>
#include <string.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "rcplane/common/io/serial.hpp"
#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/packet.hpp"

namespace rcplane {
namespace common {
namespace io {
serial::serial() {
  _packets.reserve(5);
  _fd = open(_tty.c_str(), O_RDWR | O_NOCTTY);
  if (_fd < 0) {
    RCPLANE_LOG(error, TAG, "Failed to open " << _tty);
    throw std::runtime_error("");
  }

  // Store copy of old values
  tcgetattr(_fd, &_otio);

  bzero(&_ntio, sizeof(_ntio));

  // Config the serial port
  _ntio.c_cflag = B115200 | CRTSCTS | CS8 | CLOCAL | CREAD;
  _ntio.c_iflag = IGNPAR | ICRNL;
  _ntio.c_oflag = 0;
  _ntio.c_lflag = ICANON;
  _ntio.c_cc[VINTR] = 0;
  _ntio.c_cc[VQUIT] = 0;
  _ntio.c_cc[VERASE] = 0;
  _ntio.c_cc[VKILL] = 0;
  _ntio.c_cc[VEOF] = 4;
  _ntio.c_cc[VTIME] = 0;
  _ntio.c_cc[VMIN] = 1;
  _ntio.c_cc[VSWTC] = 0;
  _ntio.c_cc[VSTART] = 0;
  _ntio.c_cc[VSTOP] = 0;
  _ntio.c_cc[VSUSP] = 0;
  _ntio.c_cc[VEOL] = 0;
  _ntio.c_cc[VREPRINT] = 0;
  _ntio.c_cc[VDISCARD] = 0;
  _ntio.c_cc[VWERASE] = 0;
  _ntio.c_cc[VLNEXT] = 0;
  _ntio.c_cc[VEOL2] = 0;

  tcflush(_fd, TCIFLUSH);
  tcsetattr(_fd, TCSANOW, &_ntio);
}

serial::~serial() {
  // Restore old values
  tcsetattr(_fd, TCSANOW, &_otio);
}

void serial::register_cb(std::function<void(std::vector<packet> &)> cb) {
  _cb = cb;
}

void serial::read_serial() {
   while (1) { 
      _res = read(_fd, _buf, MAX_LEN - 1); 
      _buf[_res]='\0'; 
      if (_res <= 1) {
        continue;
      }

      try {
        uint32_t ubuf = std::stoul(_buf, nullptr, 2);
        rcplane::common::io::packet packet(ubuf);

        switch (packet.type()) {
          case packet::type::motor:
          case packet::type::aileron:
          case packet::type::elevator:
          case packet::type::rudder:
            _packets[static_cast<int>(packet.type())] = packet;
            break;
          case packet::type::state:
            _packets[static_cast<int>(packet.type())] = packet;
            if (_cb) {
              _cb(_packets);
            }
            break;
          default:
            break;
        }
      } catch (...) {
        // Do nothing
      }

   }
}

} // namesapce io
} // namesapce common
} // namesapce rcplane