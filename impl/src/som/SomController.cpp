#include "rcplane/som/SomController.hpp"

#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/thread.hpp>
#include <memory>
#include <thread>

#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace som {

SomController::SomController() {
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

SomController::~SomController() {
  RCPLANE_LOG_METHOD();

  stopIoThread();
}

void SomController::runMainLoop() {
  RCPLANE_LOG_METHOD();

  std::this_thread::sleep_for(std::chrono::milliseconds(200));
  assert(!m_ioService.stopped());

  while (!m_ioService.stopped()) {
    const auto statePacket =
        m_serialController->readPacket<common::StatePacket>().packet;
    const auto controlSurfacePacket =
        m_serialController->readPacket<common::ControlSurfacePacket>().packet;
    const auto imuPacket =
        m_serialController->readPacket<common::ImuPacket>().packet;

    RCPLANE_LOG(debug, statePacket);
    RCPLANE_LOG(debug, controlSurfacePacket);
    RCPLANE_LOG(debug, imuPacket);

    if (!m_serialController->writePacket<common::ControlSurfacePacket>(
            controlSurfacePacket)) {
      RCPLANE_LOG(error, "Failed to write packet to MCU.");
    }
  }
}

void SomController::startIoThread() {
  RCPLANE_LOG_METHOD();

  m_ioThread = boost::thread([&]() { m_ioService.run(); });
}

void SomController::stopIoThread() {
  RCPLANE_LOG_METHOD();

  m_worker.reset();
  m_ioThread.join();
}

bool SomController::handshakeMCU() {
  RCPLANE_LOG_METHOD();

  assert(m_serialController->flush());

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
        m_serialController->readPacket<common::HandshakePacket>().packet;
        
    if (kHandshakePacket == handshakePacket) {
      RCPLANE_LOG(info, "MCU handshake succeeded!");
      return true;
    } else if (handshakePacket == common::HandshakePacket()) {
      RCPLANE_LOG(warning, "Timeout waiting for handshake packet!");
    } else {
      RCPLANE_LOG(
          info,
          "Flushed invalid handshake packet :: " << +handshakePacket.handshake);
    }
  }

  RCPLANE_LOG(error, "Handshake never acknowledged!");
  return false;
}
}  // namespace som
}  // namespace rcplane