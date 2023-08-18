#ifndef __RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__
#define __RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__

#include <array>
#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <fstream>
#include <functional>
#include <string>

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
  explicit serial_controller(boost::asio::io_service &io);
  virtual ~serial_controller();

  /**
   * @brief Connect to the serial port.
   *
   * Connect to the serial port with 115200 baudrate. Flush the serial
   * port on connection and perform a handshake with the microcontroller.
   *
   * @warning Blocks until p_handshake_mcu() and p_flush() complete.
   * @return Status of the initialization.
   */
  bool init() {
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
    
  }

  /**
   * @brief Close the serial port connection.
   * @warning Blocks until p_read_line() finishes.
   * @pre start() 
   */
  void terminate() {
    
  }

private:
  /**
   * @brief Continously read the serial port until terminate() is called.
   *
   * Handle line switching and forwarding the correct data to the correct callbacks.
   */
  virtual void read_serial() {

  }

  /**
   * @brief Open a connection to the serial port and flush the buffers.
   *
   * Open the serial port connection and generate the black box log file.
   *
   * @returns Status of openning the serial port.
   */
  virtual bool open_port() {

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
    
  }

  /**
   * @brief Read a line from the serial port.
   * @warning Possible for trailing characters to be read into _streambuffer.
   * @returns uint64_t containing the binary value of the data read or boost::optional::empty
   *          if conversion to uint64_t failed.
   */
  virtual boost::optional<uint64_t> read_line() {
    
  }

  /**
   * @brief Flush the serial buffers.
   */
  void flush();

  /**
   * @brief Handle coversion of _buffer into the designated packets.
   */
  void handle_buffer();

  std::string TTY{};
  uint32_t BAUDRATE{};
  std::ofstream _blackbox;

  uint8_t _line = 0;
  boost::asio::streambuf _streambuffer;
  uint64_t _buffer;

  boost::asio::io_service _io;
  boost::asio::serial_port _serial;
};

}  // namespace io
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__
