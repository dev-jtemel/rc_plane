#include <bitset>
#include <chrono>
#include <fcntl.h>
#include <iomanip>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <thread>
#include <unistd.h>

#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/packet.hpp"
#include "rcplane/common/io/serial_controller.hpp"

namespace rcplane {
namespace common {
namespace io {

serial_controller::serial_controller()
  : ::rcplane::common::interface::base_controller("serial") {}

serial_controller::~serial_controller() {}

bool serial_controller::init() {
  static_assert(sizeof(float) == 4U);

  _fd = open(_tty.c_str(), O_RDWR | O_NOCTTY);
  if (_fd < 0) {
    RCPLANE_LOG(error, _tag, "Failed to open " << _tty);
    return false;
  }

  std::stringstream ss;

  std::time_t now_t =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

  ss << "./logs/" << std::put_time(std::localtime(&now_t), "%F-%T")
     << ".blackbox";

  _blackbox = std::ofstream(ss.str());

  // Store copy of old values
  tcgetattr(_fd, &_otio);

  bzero(&_ntio, sizeof(_ntio));

  // Config the serial port
  _ntio.c_cflag = B115200 | CRTSCTS | CS8 | CLOCAL | CREAD;
  _ntio.c_iflag = IGNPAR | ICRNL;
  _ntio.c_lflag = ICANON;
  _ntio.c_cc[VEOF] = 4;
  _ntio.c_cc[VMIN] = 1;

  tcflush(_fd, TCIFLUSH);
  tcsetattr(_fd, TCSANOW, &_ntio);

  RCPLANE_LOG(info, _tag, "initialized");
  return true;
}

void serial_controller::register_cs_cb(
    std::function<void(uint8_t, uint8_t, int8_t, int8_t, int8_t)> cb) {
  _cs_cb = cb;
}

void serial_controller::register_gyro_cb(
    std::function<void(float, float, float)> cb) {
  _gyro_cb = cb;
}

void serial_controller::start() {
  {
    std::lock_guard<std::mutex> lk(_lk);
    _running = true;
  }

  _worker = std::thread([&]() { p_read_serial(); });

  RCPLANE_LOG(info, _tag, "started");
}

void serial_controller::terminate() {
  {
    std::lock_guard<std::mutex> lk(_lk);
    _running = false;
  }
  _worker.join();

  // Restore old values
  tcsetattr(_fd, TCSANOW, &_otio);
  _blackbox.close();

  RCPLANE_LOG(info, _tag, "terminated");
}

void serial_controller::p_read_serial() {
  while (true) {
    {
      std::lock_guard<std::mutex> lk(_lk);
      if (!_running) { break; }
    }

    _res = read(_fd, _buf, MAX_LEN - 1);
    _buf[_res] = '\0';

    if (_res <= 1) { continue; }

    try {
      _buffer = std::stoul(_buf, nullptr, 16);

      if ((START_INDICATOR == _buffer) && _startcount < 4U) {
        ++_startcount;
        continue;
      }

      if (_startcount < 4U) { continue; }

      if (_line == 0U) {
        p_handle_buffer();
      } else if (_line == 1U) {
        _pitch.set(static_cast<uint32_t>(_buffer >> 32U));
        _roll.set(static_cast<uint32_t>(0xFFFFFFFF & _buffer));
      } else if (_line == 2U) {
        _yaw.set(static_cast<uint32_t>(_buffer >> 32U));
        _gyro_cb(_pitch.data(), _roll.data(), _yaw.data());
      } else if (_line == 3U) {
      }

      _blackbox << _buf << std::endl;
      _line = (_line + 1U) % 4U;
    } catch (...) {
      // Do nothing
    }
  }
}

void serial_controller::p_handle_buffer() {
  _timestamp.set(static_cast<uint32_t>(_buffer >> 40));
  _state.set(_buffer);
  _motor.set(_buffer >> 8);
  _aileron.set(_buffer >> 16);
  _elevator.set(_buffer >> 24);
  _rudder.set(_buffer >> 32);

  RCPLANE_LOG(trace,
              _tag,
              "[" << timestamp << "]"
                  << " state = " << std::bitset<8>(_state.data())
                  << " | motor = " << +_motor.data() << " | aileron = "
                  << +_aileron.data() << " | elevator = " << +_elevator.data()
                  << " | rudder = " << +_rudder.data());

  if (_cs_cb) {
    _cs_cb(_state.data(),
           _motor.data(),
           _aileron.data(),
           _elevator.data(),
           _rudder.data());
  } else {
    RCPLANE_LOG(warn, _tag, "no cb regisetered");
  }
}

}  // namespace io
}  // namespace common
}  // namespace rcplane
