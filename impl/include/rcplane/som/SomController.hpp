#ifndef RCPLANE_SOM_SOM_CONTROLLER_HPP
#define RCPLANE_SOM_SOM_CONTROLLER_HPP

#include <boost/asio.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>
#include <memory>

#include "rcplane/autopilot/AutopilotManager.hpp"
#include "rcplane/autopilot/AutopilotUtility.hpp"
#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/Journal.hpp"
#include "rcplane/io/SerialController.hpp"
#include "rcplane/io/telemetry/TelemetryTransmitterMQ.hpp"

namespace rcplane {
namespace som {

/**
 * @brief Main controller for the SoM. Handles reading and writing to the
 * MCU and applying any processing or filtering to incoming/outgoing data.
 */
class SomController : public boost::noncopyable {
public:
  /**
   * @brief Config the SerialController and underlying data structs.
   * @param configPath The path to location of the configuration file.
   */
  SomController(const std::string &configPath = "");
  ~SomController();

  /**
   * @brief Gain reference to the underlying io context.
   * return Reference to the underlying io context.
   */
  boost::asio::io_service &getIoService();

  /**
   * @brief Run the main loop of the SoM.
   * 
   * Loop:
   * Read the 3 incoming packets, apply any filtering/processing and
   * write a control packet to the MCU.
   */
  void runMainLoop();

  /**
   * @brief Start a thread to run the m_ioService.
   */
  void startIoThread();

  /**
   * @brief Stop the thread running the m_ioService. 
   * @warning This may hang until one loop cycle in runMainLoop has completed.
   */
  void stopIoThread();

  /**
   * @brief Flush the serial buffer and perform a handshake with the MCU.
   * 
   * To ensure data is read in the correct order (and any old data is removed),
   * flush the SerialController buffer and write a handshake packet to the MCU.
   * The MCU should immediately respond with the same packet to complete the
   * handshake.
   * 
   * @return bool True if the handshake was completed, false if it timedout.
   */
  bool handshakeMCU();

private:
  /**
   * @brief Send telemetry data to the receiver.
   */
  void sendTelemetry();

  uint32_t c_handshakeAttempts{};
  uint32_t c_mainLoopDelay{};

  boost::thread m_ioThread{};
  boost::asio::io_service m_ioService{};
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
      m_worker = boost::asio::make_work_guard(m_ioService);
  boost::asio::signal_set m_signalSet{m_ioService, SIGINT};

  std::unique_ptr<io::ConfigManager> m_configManager{};
  std::unique_ptr<io::SerialController> m_serialController{};
  std::unique_ptr<autopilot::AutopilotUtility> m_autopilotUtility{};
  std::unique_ptr<autopilot::AutopilotManager> m_autopilotManager{};

  io::telemetry::message::DebugMessage m_debugMessage{};
  io::telemetry::message::AttitudeMessage m_attitudeMessage{};
  io::telemetry::message::OnboardStateMessage m_onboardMessage{};
  std::unique_ptr<io::telemetry::TelemetryTransmitterMQ>
      m_telemetryTransmitter{};
};

}  // namespace som
}  // namespace rcplane

#endif  //RCPLANE_SOM_SOM_CONTROLLER_HPP
