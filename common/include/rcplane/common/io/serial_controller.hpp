#ifndef __RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__
#define __RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__

#include <array>
#include <boost/asio.hpp>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <fstream>
#include <functional>
#include <string>

#include "rcplane/common/base_controller.hpp"
#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/packet.hpp"

namespace rcplane {
namespace common {
namespace io {

/**
 * @brief Connect and read data from the serial port.
 *
 * Controller for interacting with the serial port. Since data is received in a 
 * predefined format, callbacks can be registered to be notified of specific data
 * types when received.
 */
class serial_controller : public ::rcplane::common::interface::base_controller {
public:
  explicit serial_controller();
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
  bool init() override;

  /**
   * @brief Read the serial port.
   *
   * Spawn an internal _worker thread to begin reading the serial port.
   *
   * @pre init() succeeded.
   */
  void start() override;

  /**
   * @brief Close the serial port connection.
   * @warning Blocks until p_read_line() finishes.
   * @pre start() 
   */
  void terminate() override;

  /**
   * @brief Register a callback to be fired when a control surface statement is read.
   * @param cb The callback to register.
   */
  void register_cs_cb(
      std::function<void(uint8_t, uint8_t, int8_t, int8_t, int8_t)> cb);

  /**
   * @brief Register a callback to be fired when a gyro scope statement is read.
   * @param cb The callback to register.
   */
  void register_gyro_cb(std::function<void(float, float, float)> cb);

private:
  /**
   * @brief Continously read the serial port until terminate() is called.
   *
   * Handle line switching and forwarding the correct data to the correct callbacks.
   */
  virtual void p_read_serial();

  /**
   * @brief Open a connection to the serial port and flush the buffers.
   *
   * Open the serial port connection and generate the black box log file.
   *
   * @returns Status of openning the serial port.
   */
  virtual bool p_open_port();

  /**
   * @brief Handshake with MCU to coordinate serial io.
   *
   * Write two characters to the microcontroller to allow it to start execution.
   *
   * @see HELLO_TX.
   * @throws boost::system::system_error on write failure.
   */
  virtual bool p_handshake_mcu();

  /**
   * @brief Read a line from the serial port.
   * @warning Possible for trailing characters to be read into _streambuffer.
   * @returns uint64_t containing the binary value of the data read or boost::optional::empty
   *          if conversion to uint64_t failed.
   */
  virtual boost::optional<uint64_t> p_read_line();

  /**
   * @brief Flush the serial buffers.
   */
  void p_flush();

  /**
   * @brief Handle coversion of _buffer into the designated packets.
   */
  void p_handle_buffer();

  uint8_t _line = 0;
  const std::string TTY = "/dev/ttyACM0";
  const std::string HELLO_TX = "1";
  const std::string HELLO_RX = "rcplane\r";
  std::ofstream _blackbox;

  boost::asio::streambuf _streambuffer;
  uint64_t _buffer;
  packet<uint32_t, uint32_t> _timestamp;
  packet<uint8_t, uint8_t> _state, _motor;
  packet<uint8_t, int8_t> _aileron, _elevator, _rudder;
  packet<uint32_t, float> _pitch, _roll, _yaw;

  std::function<void(uint8_t, uint8_t, int8_t, int8_t, int8_t)> _cs_cb;
  std::function<void(float, float, float)> _gyro_cb;

  boost::asio::io_service _io;
  boost::asio::serial_port _serial;
};

}  // namespace io
}  // namespace common
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__
