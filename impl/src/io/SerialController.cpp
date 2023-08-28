#include "rcplane/io/SerialController.hpp"

#include <boost/system/error_code.hpp>
#include <chrono>
#include <future>
#include <memory>

#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace io {

SerialController::SerialController(const ConfigManager &configManager,
                                   boost::asio::io_service &io_service)
  : m_serialPort(io_service), m_ioService(io_service) {
  RCPLANE_LOG_METHOD();

  c_ttyDev = configManager.getValue<std::string>(
      "rcplane.io.serial_controller.tty_dev");
  c_baudRate = configManager.getValue<uint32_t>(
      "rcplane.io.serial_controller.baud_rate");
  c_readTimeoutMs = configManager.getValue<uint32_t>(
      "rcplane.io.serial_controller.read_timeout_ms");
  c_writeTimeoutMs = configManager.getValue<uint32_t>(
      "rcplane.io.serial_controller.write_timeout_ms");
  c_handshakeTerminationStr = configManager.getValue<std::string>(
      "rcplane.common.handshake_termination_string");
}

SerialController::~SerialController() { RCPLANE_LOG_METHOD(); }

bool SerialController::open() {
  RCPLANE_LOG_METHOD();
  try {
    m_serialPort.open(c_ttyDev.c_str());
    m_serialPort.set_option(
        boost::asio::serial_port_base::baud_rate(c_baudRate));
    RCPLANE_LOG(info,
                "Serial port opened :: " << c_ttyDev << " :: " << c_baudRate);
    return true;
  } catch (const boost::system::system_error &error) {
    RCPLANE_LOG(error,
                "Failed to open serial port :: " << c_ttyDev
                                                 << " :: " << error.what());
    return false;
  }
}

template<typename PACKET_TYPE,
         typename = std::enable_if_t<
             std::is_base_of_v<common::BasePacket, PACKET_TYPE>>>
SerialController::ReadResult<PACKET_TYPE> SerialController::readPacket() {
  RCPLANE_LOG_METHOD();

  std::promise<ReadResult<PACKET_TYPE>> readPromise;
  auto readFuture = readPromise.get_future();

  boost::asio::async_read(
      m_serialPort,
      m_streamBuffer,
      boost::asio::transfer_exactly(sizeof(PACKET_TYPE)),
      [&](const boost::system::error_code &error, std::size_t bytesRead) {
        if (error) {
          RCPLANE_LOG(error, "Failed to read packet :: " << error.message());
          return;
        }

        const auto *packetPtr = boost::asio::buffer_cast<const PACKET_TYPE *>(
            m_streamBuffer.data());
        m_streamBuffer.consume(sizeof(PACKET_TYPE));
        readPromise.set_value(ReadResult<PACKET_TYPE>{*packetPtr, false});
      });

  // Wait for the read operation to complete with a timeout
  const std::future_status status =
      readFuture.wait_for(std::chrono::milliseconds(c_readTimeoutMs));
  if (status == std::future_status::timeout) {
    // Attempt to cancel the read operation, can throw if serial disconnects between
    // the read and now.
    try {
      m_serialPort.cancel();
    } catch (...) {
      // Intentially empty.
    }
    return {};
  }

  return readFuture.get();
}

template<typename PACKET_TYPE,
         typename = typename std::enable_if_t<
             std::is_base_of_v<common::BasePacket, PACKET_TYPE>>>
bool SerialController::writePacket(const PACKET_TYPE &packet) {
  RCPLANE_LOG_METHOD();

  std::promise<void> writePromise;
  auto writeFuture = writePromise.get_future();

  boost::asio::async_write(
      m_serialPort,
      boost::asio::buffer((uint8_t *)&packet, sizeof(PACKET_TYPE)),
      [&](const boost::system::error_code &error, std::size_t) {
        if (error) {
          RCPLANE_LOG(error, "Failed to write packet :: " << error.message());
          return;
        }
        writePromise.set_value();
      });

  // Wait for the write operation to complete with a timeout
  const std::future_status status =
      writeFuture.wait_for(std::chrono::milliseconds(c_writeTimeoutMs));
  if (status == std::future_status::timeout) {
    RCPLANE_LOG(warning, "Write operation timed out.");
    try {
      m_serialPort.cancel();
    } catch (...) {
      // Intentially empty.
    }
    return false;
  }
  return true;
}

bool SerialController::flush() {
  RCPLANE_LOG_METHOD();

  std::promise<boost::system::error_code> readPromise;
  auto readFuture = readPromise.get_future();

  boost::asio::async_read_until(
      m_serialPort,
      m_streamBuffer,
      c_handshakeTerminationStr,
      [&](const boost::system::error_code &error, std::size_t size) {
        if (error) {
          RCPLANE_LOG(error, "Failed to read packet :: " << error.message());
          return; 
        }
        readPromise.set_value(error);
        // Consume everything + more
        m_streamBuffer.consume(size * 2U);
      });

  // Wait for the read operation to complete with a timeout
  constexpr uint16_t kDelayMultiplier = 20U;
  const std::future_status status = readFuture.wait_for(
      std::chrono::milliseconds(c_readTimeoutMs * kDelayMultiplier));
  if (status == std::future_status::timeout) {
    // Attempt to cancel the read operation, can throw if serial disconnects between
    // the read and now.
    try {
      m_serialPort.cancel();
    } catch (...) {}
    return false;
  }

  return readFuture.get().value() == boost::system::errc::success;
}

template SerialController::ReadResult<common::HandshakePacket>
SerialController::readPacket<common::HandshakePacket>();
template SerialController::ReadResult<common::StatePacket> SerialController::
    readPacket<common::StatePacket>();
template SerialController::ReadResult<common::ControlSurfacePacket>
SerialController::readPacket<common::ControlSurfacePacket>();
template SerialController::ReadResult<common::ImuPacket> SerialController::
    readPacket<common::ImuPacket>();

template bool SerialController::writePacket<common::HandshakePacket>(
    const common::HandshakePacket &);
template bool SerialController::writePacket<common::StatePacket>(
    const common::StatePacket &);
template bool SerialController::writePacket<common::ControlSurfacePacket>(
    const common::ControlSurfacePacket &);
template bool SerialController::writePacket<common::ImuPacket>(
    const common::ImuPacket &);
}  // namespace io
}  // namespace rcplane