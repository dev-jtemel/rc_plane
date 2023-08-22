/**
 * @file serial_controller.hpp
 * @author Jonathon Temelkovski (dev.jtemel@gmail.com)
 * @version 0.1
 * @date 2023-08-21
 */
#ifndef __RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__
#define __RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__

#include <boost/asio.hpp>
#include <boost/atomic.hpp>
#include <boost/noncopyable.hpp>
#include <boost/signals2.hpp>
#include <boost/thread.hpp>
#include <fstream>
#include <string>

#include "rcplane/base_controller.hpp"
#include "rcplane/common/packet.hpp"

namespace rcplane {
namespace io {

/**
 * @brief Connect and read data from the serial port.
 *
 * Controller for interacting with the serial port. Since data is received in a 
 * predefined format, signals can be registered to be notified of specific data
 * types when received.
 */
class serial_controller : public ::rcplane::interface::base_controller,
                          public ::boost::noncopyable {
public:
  /**
   * @brief Construct a serial controller and hold the io_service reference.
   * @param config_manager The config manager to grab config values from.
   * @param io The io_context of the main application.
   */
  explicit serial_controller(config_manager &config_manager,
                             boost::asio::io_context &io);
  ~serial_controller();

  /**
   * @brief Connect to the serial port.
   *
   * Connect to the serial port with configed baudrate. Flush the serial
   * port on connection and perform a handshake with the microcontroller.
   *
   * @warning Blocks until handshake_mcu() and flush() complete.
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
   * @warning Blocks until read_write_serial() iteration finishes.
   * @pre start() 
   */
  void terminate() override;

  /**
   * @brief Handler to post an io event to write the control surface packet
   * to the mcu.
   */
  void on_write_signal();

  /**
   * @brief Connect to a singal slot for the control surface packet.
   * @returns boost::signal<void(control_surface_packet *)> reference to
   * connect to.
   */
  boost::signals2::signal<void(common::state_packet *)> &state_signal();

  /**
   * @brief Connect to a singal slot for the control surface packet.
   * @returns boost::signal<void(control_surface_packet *)> reference to
   * connect to.
   */
  boost::signals2::signal<void(common::control_surface_packet *,
                               common::imu_packet *)>
      &packet_signal();

private:
  /**
   * @brief Continously read and write the serial port until terminate() is called.
   *
   * Handle line switching and forwarding the correct data to the correct callbacks.
   */
  void read_write_serial();

  /**
   * @brief Open a connection to the serial port and flush the buffers.
   *
   * Open the serial port connection and generate the black box log file.
   *
   * @returns Status of openning the serial port.
   */
  bool open_port();

  /**
   * @brief Handshake with MCU to coordinate serial io.
   *
   * Write one character to the microcontroller to allow it to start execution.
   *
   * @see kHELLO_TX.
   * @throws boost::system::system_error on write failure.
   */
  bool handshake_mcu();

  /**
   * @brief Read one packet from the serial bus.
   * 
   * @tparam PACKET_TYPE The type of packet to read and consume.
   * @return PACKET_TYPE* Pointer to the packet read.
   */
  template<typename PACKET_TYPE>
  PACKET_TYPE *read_packet();

  /**
   * @brief Write one packet from the serial bus.
   */
  void write_packet();

  /**
   * @brief Flush the serial buffers.
   * 
   * Awaits for a specific sequence before buffer is considered flush.
   * 
   * @see kHELLO_RX
   */
  void flush();

  std::string cTTY{};
  uint32_t cBAUDRATE{};
  std::string cBLACKBOX{};

  const std::string kHELLO_RX{"rcplane\r"};
  const uint8_t kHELLO_TX{1U};

  boost::atomic<bool> _running{false};

  boost::asio::streambuf _streambuffer;

  common::state_packet *_state_packet;
  common::control_surface_packet *_cs_packet;
  common::imu_packet *_imu_packet;

  boost::thread _worker;
  boost::asio::io_service &_io;
  boost::asio::serial_port _serial;
  boost::signals2::signal<void(common::state_packet *)> _state_signal;
  boost::signals2::signal<void(common::control_surface_packet *,
                               common::imu_packet *)>
      _packet_signal;

  std::ofstream _blackbox_in;
  std::ofstream _blackbox_out;
};

}  // namespace io
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__IO__SERIAL_CONTROLLER_HPP__