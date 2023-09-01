#include "rcplane/io/telemetry/TelemetryTransmitterMQ.hpp"

#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace io {
namespace telemetry {

TelemetryTransmitterMQ::TelemetryTransmitterMQ(
    const ConfigManager &configManager) {
  RCPLANE_LOG_METHOD();

  c_mQueueName =
      configManager.getValue<std::string>("rcplane.io.telemetry.debug_mq_name");
  c_mQueueSize =
      configManager.getValue<uint32_t>("rcplane.io.telemetry.debug_mq_size");
}

TelemetryTransmitterMQ::~TelemetryTransmitterMQ() {
  RCPLANE_LOG_METHOD();
  boost::interprocess::message_queue::remove(c_mQueueName.c_str());
}

bool TelemetryTransmitterMQ::init() {
  RCPLANE_LOG_METHOD();
  try {
    m_mQueue = std::make_unique<boost::interprocess::message_queue>(
        boost::interprocess::open_or_create,
        c_mQueueName.c_str(),
        c_mQueueSize,
        sizeof(message::DebugMessage));
    return true;
  } catch (boost::interprocess::interprocess_exception &e) {
    RCPLANE_LOG(error, "Failed to create message queue: " << e.what());
    return false;
  }
}

bool TelemetryTransmitterMQ::sendDebugMessage(
    const message::DebugMessage &message) {
  RCPLANE_LOG_METHOD();
  m_mQueue->send((uint8_t *)&message, sizeof(message), 0);
  return true;
}

}  // namespace telemetry
}  // namespace io
}  // namespace rcplane