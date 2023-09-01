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

SomController::SomController(const std::string &configPath) {
  m_signalSet.async_wait([&](boost::system::error_code ec, int sig) {
    RCPLANE_LOG(warning, "Termination signal received!");
    m_worker.reset();
  });

  m_configManager = std::make_unique<io::ConfigManager>();
  if (configPath.empty()) {
    assert(m_configManager->loadConfig());
  } else {
    assert(m_configManager->loadConfig(configPath));
  }

  RCPLANE_LOG(debug, m_configManager->dumpConfig());

  RCPLANE_LOG(info, "ConfigManager initialized!");

  c_handshakeAttempts = m_configManager->getValue<uint32_t>(
      "rcplane.som.som_controller.handshake_attempts");
  c_mainLoopDelay = m_configManager->getValue<uint32_t>(
      "rcplane.som.som_controller.main_loop_delay_ms");

  m_serialController =
      std::make_unique<io::SerialController>(*m_configManager.get(),
                                             m_ioService);
  assert(m_serialController->open());

  RCPLANE_LOG(info, "SerialController initialized!");

  m_autopilotUtility =
      std::make_unique<autopilot::AutopilotUtility>(*m_configManager.get());
  m_autopilotManager =
      std::make_unique<autopilot::AutopilotManager>(*m_autopilotUtility.get());
  RCPLANE_LOG(info, "Autopilot initialized!");

  m_telemetryTransmitterMQ =
      std::make_unique<io::telemetry::TelemetryTransmitterMQ>(
          *m_configManager.get());

  assert(m_telemetryTransmitterMQ->init());
  RCPLANE_LOG(info, "Telemetry transmitter initialized!");
}

SomController::~SomController() {
  RCPLANE_LOG_METHOD();

  stopIoThread();
}

boost::asio::io_service &SomController::getIoService() { return m_ioService; }

void SomController::runMainLoop() {
  RCPLANE_LOG_METHOD();

  sendTelemetry();

  std::this_thread::sleep_for(std::chrono::milliseconds(c_mainLoopDelay));
  assert(!m_ioService.stopped());

  while (!m_ioService.stopped()) {
    const auto kRcRxPacketResult =
        m_serialController->readPacket<common::RcRxPacket>();

    kRcRxPacketResult.didTimeout ? ++m_debugMessage.serialReadTimeouts
                                 : ++m_debugMessage.serialReads;

    const auto kImuPacketResult =
        m_serialController->readPacket<common::ImuPacket>();

    kImuPacketResult.didTimeout ? ++m_debugMessage.serialReadTimeouts
                                : ++m_debugMessage.serialReads;

    RCPLANE_LOG(debug, kRcRxPacketResult.packet);
    RCPLANE_LOG(debug, kImuPacketResult.packet);

    common::ControlSurfacePacket controlSurfacePacket =
        m_autopilotManager->trigger(kRcRxPacketResult.packet,
                                    kImuPacketResult.packet);

    RCPLANE_LOG(debug, controlSurfacePacket);

    if (!m_serialController->writePacket<common::ControlSurfacePacket>(
            controlSurfacePacket)) {
      RCPLANE_LOG(error, "Failed to write packet to MCU.");
      ++m_debugMessage.serialWriteTimeouts;
    } else {
      ++m_debugMessage.serialWrites;
    }

    ++m_debugMessage.mainLoopCounter;

    sendTelemetry();
  }
}

void SomController::startIoThread() {
  RCPLANE_LOG_METHOD();

  m_ioThread = boost::thread([&]() { m_ioService.run(); });
}

void SomController::stopIoThread() {
  RCPLANE_LOG_METHOD();

  m_worker.reset();
  m_worker.~executor_work_guard();
  m_worker.~executor_work_guard();
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
  } else {
    RCPLANE_LOG(debug, "Handshake packet written!");
    ++m_debugMessage.serialWrites;
  }

  for (uint32_t i = 0; i < c_handshakeAttempts; ++i) {
    const auto kHandshakePacketResult =
        m_serialController->readPacket<common::HandshakePacket>();

    if (kHandshakePacketResult.didTimeout) {
      ++m_debugMessage.serialReadTimeouts;
      continue;
    }

    if (kHandshakePacket == kHandshakePacketResult.packet) {
      ++m_debugMessage.serialReads;
      RCPLANE_LOG(info, "MCU handshake succeeded!");
      return true;
    } else {
      RCPLANE_LOG(info,
                  "Flushed invalid handshake packet :: "
                      << +kHandshakePacketResult.packet.handshake);
    }
  }

  RCPLANE_LOG(error, "Handshake never acknowledged!");
  return false;
}

void SomController::sendTelemetry() {
  RCPLANE_LOG_METHOD();

  if (!m_telemetryTransmitterMQ->sendDebugMessage(m_debugMessage)) {
    RCPLANE_LOG(error, "Failed to send telemetry!");
  }
}

}  // namespace som
}  // namespace rcplane