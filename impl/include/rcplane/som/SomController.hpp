#ifndef RCPLANE_SOM_SOM_CONTROLLER_HPP
#define RCPLANE_SOM_SOM_CONTROLLER_HPP

#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <memory>

#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/Journal.hpp"
#include "rcplane/io/SerialController.hpp"

namespace rcplane {
namespace som {

class SomController : public boost::noncopyable {
public:
  SomController() {
    RCPLANE_LOG_METHOD();

    m_signalSet.async_wait([&](boost::system::error_code ec, int sig) {
      RCPLANE_LOG(warning, "Termination signal received!");
      m_worker.reset();
    });

    m_configManager = std::make_unique<io::ConfigManager>();
    assert(m_configManager->loadConfig());
    RCPLANE_LOG(debug, m_configManager->dumpConfig());

    RCPLANE_LOG(info, "ConfigManager initialized!");

    m_serialController =
        std::make_unique<io::SerialController>(*m_configManager.get(),
                                               m_ioService);
    assert(m_serialController->open());

    RCPLANE_LOG(info, "SerialController initialized!");
  }

  ~SomController() {
    RCPLANE_LOG_METHOD();

    stopIoThread();
  }

  void runMainLoop() {
    RCPLANE_LOG_METHOD();
    assert(!m_ioService.stopped());

    while (!m_ioService.stopped()) {
      const auto statePacket = m_serialController->readPacket<common::StatePacket>();
      const auto controlSurfacePacket = m_serialController->readPacket<common::ControlSurfacePacket>();
      
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      RCPLANE_LOG(debug, "main loop");
    }
  }

  void startIoThread() {
    RCPLANE_LOG_METHOD();

    m_ioThread = boost::thread([&]() { m_ioService.run(); });
  }

  void stopIoThread() {
    RCPLANE_LOG_METHOD();

    m_worker.reset();
    m_ioThread.join();
  }

  bool handshakeMCU() {
    RCPLANE_LOG_METHOD();

    const common::HandshakePacket kHandshakePacket{1U};
    if (!m_serialController->writePacket<common::HandshakePacket>(
            kHandshakePacket)) {
      RCPLANE_LOG(error, "Failed to write handshake packet!");
      return false;
    }

    constexpr uint8_t kHandshakeAttempts = 5U;
    for (uint8_t i = 0; i < kHandshakeAttempts; ++i) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      const auto handshakePacket =
          m_serialController->readPacket<common::HandshakePacket>();
      RCPLANE_LOG(error, +handshakePacket.handshake);
      if (kHandshakePacket == handshakePacket) {
        RCPLANE_LOG(info, "MCU handshake succeeded!");
        return true;
      } else if (handshakePacket == common::HandshakePacket()) {
        RCPLANE_LOG(debug, "Timeout waiting for handshake packet!");
      } else {
        RCPLANE_LOG(info,
                    "Flushed invalid handshake packet :: "
                        << +handshakePacket.handshake);
      }
    }

    RCPLANE_LOG(error, "Handshake never acknowledged!");
    return false;
  }

private:
  boost::thread m_ioThread{};
  boost::asio::io_service m_ioService{};
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
      m_worker = boost::asio::make_work_guard(m_ioService);
  boost::asio::signal_set m_signalSet{m_ioService, SIGINT};

  std::unique_ptr<io::ConfigManager> m_configManager{};
  std::unique_ptr<io::SerialController> m_serialController{};
};

}  // namespace som
}  // namespace rcplane

#endif  //RCPLANE_SOM_SOM_CONTROLLER_HPP