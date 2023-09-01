#include "rcplane/io/telemetry/TelemetryReceiverMQ.hpp"

#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace io {
namespace telemetry {

TelemetryReceiverMQ::TelemetryReceiverMQ(const ConfigManager &configManager) {
  RCPLANE_LOG_METHOD();

  c_mQueueName =
      configManager.getValue<std::string>("rcplane.io.telemetry.debug_mq_name");
}

TelemetryReceiverMQ::~TelemetryReceiverMQ() { RCPLANE_LOG_METHOD(); }

bool TelemetryReceiverMQ::init() {
  RCPLANE_LOG_METHOD();
  try {
    m_mQueue = std::make_unique<boost::interprocess::message_queue>(
        boost::interprocess::open_only,
        c_mQueueName.c_str());
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
    m_mQueue->receive(&message, sizeof(message), messageSize, priority);

    RCPLANE_LOG(info,
                "Received message: " << priority << " " << messageSize << "\n"
                                     << message);
  } catch (boost::interprocess::interprocess_exception &ex) {
    RCPLANE_LOG(error, "Failed to receive message: " << ex.what());
    return false;
  }
  return true;
}

}  // namespace telemetry
}  // namespace io
}  // namespace rcplane