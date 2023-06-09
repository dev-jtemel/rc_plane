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
#include <iomanip>
#include <thread>
#include <sstream>

#include "rcplane/common/io/serial_controller.hpp"
#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/packet.hpp"

namespace rcplane {
namespace common {
namespace io {

serial_controller::serial_controller() : ::rcplane::common::interface::base_controller("serial") {
}

serial_controller::~serial_controller() {
}

bool serial_controller::init() {
  _packets[0] = packet(packet::type::state, 0U);
  _packets[1] = packet(packet::type::motor, 0U);
  _packets[2] = packet(packet::type::aileron, 0U);
  _packets[3] = packet(packet::type::elevator, 0U);
  _packets[4] = packet(packet::type::rudder, 0U);

#ifndef SIMULATION
  _fd = open(_tty.c_str(), O_RDWR | O_NOCTTY);
  if (_fd < 0) {
    RCPLANE_LOG(error, _tag, "Failed to open " << _tty);
    return false;
  }

  std::stringstream ss;

  std::time_t now_t = std::chrono::system_clock::to_time_t(
    std::chrono::system_clock::now()
  );

  ss << "/home/pi/logs/" << std::put_time(std::localtime(&now_t), "%F-%T") << ".blackbox";

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

  RCPLANE_LOG(info, _tag, "initialized");
  return true;
}


void serial_controller::register_cb(std::function<void(uint32_t, std::array<packet, 5U> &)> cb) {
  _cb = cb;
}

void serial_controller::start() {
  {
    std::lock_guard<std::mutex> lk(_lk);
    _running = true;
  }

  _worker = std::thread([&](){
#ifdef SIMULATION
    p_read_log();
#else
    p_read_serial();
#endif
  });

  RCPLANE_LOG(info, _tag, "started");
}

void serial_controller::terminate() {
  {
    std::lock_guard<std::mutex> lk(_lk);
    _running = false;
  }
  _worker.join();

#ifndef SIMULATION
  // Restore old values
  tcsetattr(_fd, TCSANOW, &_otio);
  _blackbox.close();
#endif

  RCPLANE_LOG(info, _tag, "terminated");
}

void serial_controller::p_read_serial() {
#ifndef SIMULATION
  while (true) { 
    {
      std::lock_guard<std::mutex> lk(_lk);
      if (!_running) {
        break;
      }
    }

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

void serial_controller::p_read_log() {
#ifdef SIMULATION
  std::string line;
  uint32_t lastSeenTime = 0U;
  while (_log >> line) {
    {
      std::lock_guard<std::mutex> lk(_lk);
      if (!_running) {
        break;
      }
    }
    try {
      _buffer = std::stoul(line.c_str(), nullptr, 16);
      auto timestamp = static_cast<uint32_t>(_buffer >> 40);

      std::this_thread::sleep_for(std::chrono::milliseconds(timestamp - lastSeenTime));

      p_handle_buffer();

      lastSeenTime = timestamp;
    } catch (...) {
      RCPLANE_LOG(error, _tag, "failed to convert line: " << line);
    }
  }
#endif
}

void serial_controller::p_handle_buffer() {
  auto timestamp = static_cast<uint32_t>(_buffer >> 40);
  _packets[0].set(_buffer);
  _packets[1].set(_buffer >> 8);
  _packets[2].set(_buffer >> 16);
  _packets[3].set(_buffer >> 24);
  _packets[4].set(_buffer >> 32);

  RCPLANE_LOG(
    trace,
    _tag,
    "[" << timestamp << "]"
    << " state = " << std::bitset<8>(_packets[0].data())
    << " | motor = " << _packets[1].data()
    << " | aileron = " << _packets[2].data()
    << " | elevator = " << _packets[3].data()
    << " | rudder = " << _packets[4].data()
  ); 

  if (_cb) {
    _cb(timestamp, _packets);
  } else {
    RCPLANE_LOG(warn, _tag, "no cb regisetered");
  }
}

} // namesapce io
} // namesapce common
} // namesapce rcplane
