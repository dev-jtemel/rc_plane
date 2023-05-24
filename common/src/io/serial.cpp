#include "rcplane/common/io/serial.hpp"
#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/packet.hpp"

namespace rcplane {
namespace common {
namespace io {
serial::serial() {
  _fd = open(_tty.c_str(), O_RDWR | O_NOCTTY);
  if (_fd < 0) {
    RCPLANE_LOG(error, TAG, "Failed to open " << _tty);
    throw std::runtime_error("FD");
  }

  tcgetattr(_fd,&oldtio);
  bzero(&newtio, sizeof(newtio));
  newtio.c_cflag = B115200 | CRTSCTS | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNPAR | ICRNL;
  newtio.c_oflag = 0;
  newtio.c_lflag = ICANON;
  newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
  newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
  newtio.c_cc[VERASE]   = 0;     /* del */
  newtio.c_cc[VKILL]    = 0;     /* @ */
  newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */
  newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
  newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
  newtio.c_cc[VSWTC]    = 0;     /* '\0' */
  newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
  newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
  newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
  newtio.c_cc[VEOL]     = 0;     /* '\0' */
  newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
  newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
  newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
  newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
  newtio.c_cc[VEOL2]    = 0;     /* '\0' */
  tcflush(_fd, TCIFLUSH);
  tcsetattr(_fd,TCSANOW,&newtio);
}
serial::~serial() {
  tcsetattr(_fd,TCSANOW,&oldtio);
}

void serial::read_serial() {
   while (1) { 
      res = read(_fd,buf, MAX_LEN - 1); 
      buf[res]='\0'; 
      if (res <= 1) {
        continue;
      }
      try {
        uint32_t cbuf = std::stoul(buf, nullptr, 2);
        rcplane::common::io::packet packet(cbuf);
        RCPLANE_LOG(trace, "packet", packet.type_to_str() << " " << packet.data());
      } catch (...) {
        // Do nothing
      }

   }
}

} // namesapce io
} // namesapce common
} // namesapce rcplane
