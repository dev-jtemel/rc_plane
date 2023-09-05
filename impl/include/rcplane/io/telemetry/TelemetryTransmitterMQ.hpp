#ifndef RCPLANE_IO_TELEMETRY_TELEMETRY_TRANSMITTER_MQ_HPP
#define RCPLANE_IO_TELEMETRY_TELEMETRY_TRANSMITTER_MQ_HPP

#include <boost/interprocess/ipc/message_queue.hpp>

#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/telemetry/ITelemetryTransmitter.hpp"
#include "rcplane/io/telemetry/TelemetryMessage.hpp"

namespace rcplane {
namespace io {
namespace telemetry {

/**
 * @brief Implementation of the TelemetryTransmitter interface that uses a
 * message queue to local IPC.
 */
class TelemetryTransmitterMQ : public ITelemetryTransmitter {
public:
  /**
   * @brief Construct a new Telemetry Transmitter MQ object.
   * @param configManager The configuration manager.
   */
  TelemetryTransmitterMQ(const ConfigManager &configManager);
  ~TelemetryTransmitterMQ();

  /**
   * @brief Initialize the telemetry transmitter.
   * @return True if mq is initialized successfully, false otherwise.
   */
  bool init() override;

  /**
   * @brief Send a debug telemetry message.
   * @param message The debug telemetry message to send.
   * @return bool Status of the operation.
   */
  bool sendDebugMessage(const message::DebugMessage &message) override;

  /**
   * @brief Send an attitude telemetry message.
   * @param message The attitude telemetry message to send.
   * @return bool Status of the operation.
   */
  bool sendAttitudeMessage(const message::AttitudeMessage &message) override;

private:
  std::string c_debugMessageQueueName{};
  std::string c_attitudeMessageQueueName{};
  uint32_t c_messageQueueSize{0U};
  std::unique_ptr<boost::interprocess::message_queue> m_debugMessageQueue{};
  std::unique_ptr<boost::interprocess::message_queue> m_attitudeMessageQueue{};
};

}  // namespace telemetry
}  // namespace io
}  // namespace rcplane

#endif  //RCPLANE_IO_TELEMETRY_TELEMETRY_TRANSMITTER_MQ_HPP