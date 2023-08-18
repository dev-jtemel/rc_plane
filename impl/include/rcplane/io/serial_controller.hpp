#ifndef __RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__
#define __RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__

#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

#include "rcplane/base_controller.hpp"
#include "rcplane/io/journal.hpp"

namespace rcplane {
namespace io {

/**
 * @brief Connect and read data from the serial port.
 *
 * Controller for interacting with the serial port. Since data is received in a 
 * predefined format, callbacks can be registered to be notified of specific data
 * types when received.
 */
class serial_controller : public ::rcplane::interface::base_controller {
public:
  /**
   * @brief Construct a serial controller and hold the io_service reference.=
   * @param io The io_context of the main application.
   */
  explicit serial_controller(boost::asio::io_context &io)
  : interface::base_controller("serial-controller"), _io(io), _serial(_io) {
    RCPLANE_ENTER();

    TTY = config_manager::instance().get<std::string>(
        "common.io.serial_controller.dev");
    BAUDRATE = config_manager::instance().get<uint32_t>(
        "common.io.serial_controller.baudrate");
  }

  virtual ~serial_controller() {
    RCPLANE_ENTER();
  }

  /**
   * @brief Connect to the serial port.
   *
   * Connect to the serial port with 115200 baudrate. Flush the serial
   * port on connection and perform a handshake with the microcontroller.
   *
   * @warning Blocks until handshake_mcu() and flush() complete.
   * @return Status of the initialization.
   */
  bool init() {
    RCPLANE_ENTER();

    static_assert(sizeof(float) == 4U);

    if (!open_port()) { return false; }

    if (!handshake_mcu()) { return false; }

    RCPLANE_LOG(info, _tag, "initialized");
    return true;
  }

  /**
   * @brief Read the serial port.
   *
   * Spawn an internal _worker thread to begin reading the serial port.
   *
   * @pre init() succeeded.
   */
  void start() {
    RCPLANE_ENTER();
    _running = true;
    _worker = boost::thread(&serial_controller::read_serial, this);
    RCPLANE_LOG(info, _tag, "started");
  }

  /**
   * @brief Close the serial port connection.
   * @warning Blocks until p_read_line() finishes.
   * @pre start() 
   */
  void terminate() {
    RCPLANE_ENTER();
    _running = false;
  }

private:
  /**
   * @brief Continously read the serial port until terminate() is called.
   *
   * Handle line switching and forwarding the correct data to the correct callbacks.
   */
  virtual void read_serial() {
    RCPLANE_ENTER();

    while (_running) {
      auto read_buffer = read_line();
      if (!read_buffer) { continue; }
    }
  }

  /**
   * @brief Open a connection to the serial port and flush the buffers.
   *
   * Open the serial port connection and generate the black box log file.
   *
   * @returns Status of openning the serial port.
   */
  virtual bool open_port() {
    RCPLANE_ENTER();

    try {
      _serial.open(TTY);
      _serial.set_option(boost::asio::serial_port_base::baud_rate(BAUDRATE));

      ::tcflush(_serial.lowest_layer().native_handle(), TCIFLUSH);
      flush();
      return true;
    } catch (...) {
      RCPLANE_LOG(error, _tag, "failed to open serial port");
      return false;
    }
  }

  /**
   * @brief Handshake with MCU to coordinate serial io.
   *
   * Write two characters to the microcontroller to allow it to start execution.
   *
   * @see HELLO_TX.
   * @throws boost::system::system_error on write failure.
   */
  virtual bool handshake_mcu() {
    RCPLANE_ENTER();

    const auto k_write_delay = 1U;
    for (uint8_t i = 0U; i < 2U; ++i) {
      boost::asio::write(_serial,
                        boost::asio::buffer(kHELLO_TX.c_str(), kHELLO_RX.size()));
      std::this_thread::sleep_for(std::chrono::seconds(k_write_delay));
    }
    return true;
  }

  /**
   * @brief Read a line from the serial port.
   * @warning Possible for trailing characters to be read into _streambuffer.
   * @returns uint64_t containing the binary value of the data read or boost::optional::empty
   *          if conversion to uint64_t failed.
   */
  virtual boost::optional<uint64_t> read_line() {
    RCPLANE_ENTER();

    boost::asio::read_until(_serial, _streambuffer, '\n');

    std::string res{};
    std::istream is(&_streambuffer);
    std::getline(is, res);

    _blackbox << res << std::endl;
    RCPLANE_LOG(info, _tag, res);

    try {
      return {std::stoul(res, nullptr, 16)};
    } catch (...) { return {}; }
  }

  /**
   * @brief Flush the serial buffers.
   */
  void flush() {
    RCPLANE_ENTER();

    std::string res{};
    while (res != kHELLO_RX) {
      boost::asio::read_until(_serial, _streambuffer, '\n');
      std::istream is(&_streambuffer);
      std::getline(is, res);
    }
    RCPLANE_LOG(info, _tag, "flushed");
  }

  std::string TTY{};
  uint32_t BAUDRATE{};
  const std::string kHELLO_RX{"rcplane\r"};
  const std::string kHELLO_TX{"1"};
  std::atomic<bool> _running{false};
  std::ofstream _blackbox;

  uint8_t _line = 0;
  boost::asio::streambuf _streambuffer;
  uint64_t _buffer;

  boost::thread _worker;
  boost::asio::io_service &_io;
  boost::asio::serial_port _serial;
};

}  // namespace io
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__
