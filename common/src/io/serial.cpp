#include <strings.h>
#include <string.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <bitset>
#include <chrono>
#include <thread>
#include <sstream>

#include "rcplane/common/io/serial.hpp"
#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/packet.hpp"

namespace rcplane {
namespace common {
namespace io {

serial::serial() {
  _packets[0] = packet(packet::type::state, 0U);
  _packets[1] = packet(packet::type::motor, 0U);
  _packets[2] = packet(packet::type::aileron, 0U);
  _packets[3] = packet(packet::type::elevator, 0U);
  _packets[4] = packet(packet::type::rudder, 0U);

#ifndef SIMULATION
  _fd = open(_tty.c_str(), O_RDWR | O_NOCTTY);
  if (_fd < 0) {
    RCPLANE_LOG(error, TAG, "Failed to open " << _tty);
    throw std::runtime_error("");
  }

  std::stringstream ss;
  ss << "./logs/" << std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch()
  ).count() << ".log";

  _blackbox = std::ofstream(ss.str());

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
#endif
}

serial::~serial() {
#ifndef SIMULATION
  // Restore old values
  tcsetattr(_fd, TCSANOW, &_otio);
  _blackbox.close();
#endif
}

void serial::register_cb(std::function<void(uint32_t, std::array<packet, 5U> &)> cb) {
  _cb = cb;
}

void serial::read_serial() {
#ifdef SIMULATION
  p_read_log();
#else
  p_read_serial();
#endif
}

void serial::p_read_serial() {
#ifndef SIMULATION
  while (1) { 
    _res = read(_fd, _buf, MAX_LEN - 1); 
    _buf[_res]='\0'; 

    if (_res <= 1) {
      continue;
    }

    try {
      _buffer = std::stoul(_buf, nullptr, 16);
      p_handle_buffer();
      _blackbox << _buf << std::endl;
    } catch (...) {
      // Do nothing
    }
  }
#endif
}

void serial::p_read_log() {
#ifdef SIMULATION
  std::string line;
  uint32_t lastSeenTime = 0U;
  while (_log >> line) {
    try {
      _buffer = std::stoul(line.c_str(), nullptr, 16);
      auto timestamp = static_cast<uint32_t>(_buffer >> 40);

      std::this_thread::sleep_for(std::chrono::milliseconds(timestamp - lastSeenTime));

      p_handle_buffer();

      lastSeenTime = timestamp;
    } catch (...) {
      RCPLANE_LOG(error, TAG, "failed to convert line: " << line);
    }
  }
#endif
}

void serial::p_handle_buffer() {
  _packets[0].set(_buffer);
  _packets[1].set(_buffer >> 8);
  _packets[2].set(_buffer >> 16);
  _packets[3].set(_buffer >> 24);
  _packets[4].set(_buffer >> 32);
  if (_cb) {
    _cb(static_cast<uint32_t>(_buffer >> 40), _packets);
  } else {
    RCPLANE_LOG(warn, TAG, "no cb regisetered");
  }
}

} // namesapce io
} // namesapce common
} // namesapce rcplane
