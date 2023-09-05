#ifndef RCPLANE_IO_TELEMETRY_TELEMETRY_RECEIVER_MQ_HPP
#define RCPLANE_IO_TELEMETRY_TELEMETRY_RECEIVER_MQ_HPP

#include <boost/interprocess/ipc/message_queue.hpp>

#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/telemetry/ITelemetryReceiver.hpp"
#include "rcplane/io/telemetry/TelemetryMessage.hpp"

namespace rcplane {
namespace io {
namespace telemetry {

/**
 * @brief Implementation of the TelemetryReceiver interface that uses a
 * message queue to local IPC.
 */
class TelemetryReceiverMQ : public ITelemetryReceiver {
public:
  /**
   * @brief Construct a new Telemetry Receiver MQ object.
   * @param configManager The configuration manager.
   */
  TelemetryReceiverMQ(const ConfigManager &configManager);
  ~TelemetryReceiverMQ();

  /**
   * @brief Initialize the telemetry receiver.
   * @return True if MQ is initialized successfully, false otherwise.
   */
  bool init();

  /**
   * @brief Receive a debug telemetry message.
   * @return bool Status of the read operation.
   */
  bool receiveDebugMessage() override;

  /**
   * @brief Receive an attitude telemetry message.
   * @return bool Status of the read operation.
   */
  bool receiveAttitudeMessage() override;

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

#endif  //RCPLANE_IO_TELEMETRY_TELEMETRY_RECEIVER_MQ_HPP