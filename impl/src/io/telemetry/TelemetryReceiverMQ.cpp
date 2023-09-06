#include "rcplane/io/telemetry/TelemetryReceiverMQ.hpp"

#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace io {
namespace telemetry {

TelemetryReceiverMQ::TelemetryReceiverMQ(const ConfigManager &configManager)
  : ITelemetryReceiver() {
  RCPLANE_LOG_METHOD();

  c_debugMessageQueueName =
      configManager.getValue<std::string>("rcplane.io.telemetry.debug_mq_name");
  c_attitudeMessageQueueName = configManager.getValue<std::string>(
      "rcplane.io.telemetry.attitude_mq_name");
}

TelemetryReceiverMQ::~TelemetryReceiverMQ() { RCPLANE_LOG_METHOD(); }

bool TelemetryReceiverMQ::init() {
  RCPLANE_LOG_METHOD();
  try {
    m_debugMessageQueue = std::make_unique<boost::interprocess::message_queue>(
        boost::interprocess::open_only,
        c_debugMessageQueueName.c_str());
    RCPLANE_LOG(info,
                "Opened debug message queue: " << c_debugMessageQueueName);

    m_attitudeMessageQueue =
        std::make_unique<boost::interprocess::message_queue>(
            boost::interprocess::open_only,
            c_attitudeMessageQueueName.c_str());
    RCPLANE_LOG(
        info,
        "Opened attitude message queue: " << c_attitudeMessageQueueName);

    return true;
  } catch (boost::interprocess::interprocess_exception &e) {
    RCPLANE_LOG(error, "Failed to open message queue: " << e.what());
    return false;
  }
}

bool TelemetryReceiverMQ::receiveDebugMessage() {
  RCPLANE_LOG_METHOD();

  uint32_t priority;
  boost::interprocess::message_queue::size_type messageSize;
  message::DebugMessage message;

  try {
    m_debugMessageQueue->receive(&message,
                                 sizeof(message::DebugMessage),
                                 messageSize,
                                 priority);

    RCPLANE_LOG(info,
                "Received message: " << priority << " " << messageSize << "\n"
                                     << message);
  } catch (boost::interprocess::interprocess_exception &ex) {
    RCPLANE_LOG(error, "Failed to receive message: " << ex.what());
    return false;
  }
  return true;
}

bool TelemetryReceiverMQ::receiveAttitudeMessage() {
  RCPLANE_LOG_METHOD();

  uint32_t priority;
  boost::interprocess::message_queue::size_type messageSize;
  message::AttitudeMessage message;

  try {
    m_attitudeMessageQueue->receive(&message,
                                    sizeof(message::AttitudeMessage),
                                    messageSize,
                                    priority);

    RCPLANE_LOG(info,
                "Received message: " << priority << " " << messageSize << "\n"
                                     << message);
  } catch (boost::interprocess::interprocess_exception &ex) {
    RCPLANE_LOG(error, "Failed to attitude message: " << ex.what());
    return false;
  }
  return true;
}

}  // namespace telemetry
}  // namespace io
}  // namespace rcplane