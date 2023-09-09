#include "rcplane/io/telemetry/TelemetryTransmitterMQ.hpp"

#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace io {
namespace telemetry {

TelemetryTransmitterMQ::TelemetryTransmitterMQ(
    const ConfigManager &configManager)
  : ITelemetryTransmitter() {
  RCPLANE_LOG_METHOD();

  c_debugMessageQueueName =
      configManager.getValue<std::string>("rcplane.io.telemetry.debug_mq_name");
  c_attitudeMessageQueueName = configManager.getValue<std::string>(
      "rcplane.io.telemetry.attitude_mq_name");
  c_onboardMessageQueueName = configManager.getValue<std::string>(
      "rcplane.io.telemetry.onboard_mq_name");
  c_messageQueueSize =
      configManager.getValue<uint32_t>("rcplane.io.telemetry.mq_size");
}

TelemetryTransmitterMQ::~TelemetryTransmitterMQ() {
  RCPLANE_LOG_METHOD();
  boost::interprocess::message_queue::remove(c_debugMessageQueueName.c_str());
  RCPLANE_LOG(info, "Removed debug message queue: " << c_debugMessageQueueName);
  boost::interprocess::message_queue::remove(
      c_attitudeMessageQueueName.c_str());
  RCPLANE_LOG(info,
              "Removed attitude message queue: " << c_attitudeMessageQueueName);
  boost::interprocess::message_queue::remove(c_onboardMessageQueueName.c_str());
  RCPLANE_LOG(info,
              "Removed onboard message queue: " << c_onboardMessageQueueName);
}

bool TelemetryTransmitterMQ::init() {
  RCPLANE_LOG_METHOD();
  try {
    m_debugMessageQueue = std::make_unique<boost::interprocess::message_queue>(
        boost::interprocess::open_or_create,
        c_debugMessageQueueName.c_str(),
        c_messageQueueSize,
        sizeof(message::DebugMessage));

    RCPLANE_LOG(info,
                "Opened debug message queue: " << c_debugMessageQueueName);

    m_attitudeMessageQueue =
        std::make_unique<boost::interprocess::message_queue>(
            boost::interprocess::open_or_create,
            c_attitudeMessageQueueName.c_str(),
            c_messageQueueSize,
            sizeof(message::AttitudeMessage));

    RCPLANE_LOG(
        info,
        "Opened attitude message queue: " << c_attitudeMessageQueueName);

    m_onboardMessageQueue =
        std::make_unique<boost::interprocess::message_queue>(
            boost::interprocess::open_or_create,
            c_onboardMessageQueueName.c_str(),
            c_messageQueueSize,
            sizeof(message::OnboardStateMessage));

    RCPLANE_LOG(info,
                "Opened attitude message queue: " << c_onboardMessageQueueName);
    return true;
  } catch (boost::interprocess::interprocess_exception &e) {
    RCPLANE_LOG(error, "Failed to create message queue: " << e.what());
    return false;
  }
}

bool TelemetryTransmitterMQ::sendDebugMessage(
    const message::DebugMessage &message) {
  RCPLANE_LOG_METHOD();
  try {
    if (!m_debugMessageQueue->try_send((uint8_t *)&message,
                                       sizeof(message::DebugMessage),
                                       0)) {
      RCPLANE_LOG(error, "Failed to send debug message");
      return false;
    }
  } catch (const boost::interprocess::interprocess_exception &e) {
    RCPLANE_LOG(error, "Failed to send debug message: " << e.what());
    return false;
  }
  return true;
}

bool TelemetryTransmitterMQ::sendAttitudeMessage(
    const message::AttitudeMessage &message) {
  RCPLANE_LOG_METHOD();
  try {
    if (!m_attitudeMessageQueue->try_send((uint8_t *)&message,
                                          sizeof(message::AttitudeMessage),
                                          0)) {
      RCPLANE_LOG(error, "Failed to send attitude message");
      return false;
    }
  } catch (const boost::interprocess::interprocess_exception &e) {
    RCPLANE_LOG(error, "Failed to send attitude message :: " << e.what());
    return false;
  }
  return true;
}

bool TelemetryTransmitterMQ::sendOnboardMessage(
    const message::OnboardStateMessage &message) {
  RCPLANE_LOG_METHOD();
  try {
    if (!m_onboardMessageQueue->try_send((uint8_t *)&message,
                                         sizeof(message::OnboardStateMessage),
                                         0)) {
      RCPLANE_LOG(error, "Failed to send onboard message");
      return false;
    }
  } catch (const boost::interprocess::interprocess_exception &e) {
    RCPLANE_LOG(error, "Failed to send onboard message :: " << e.what());
    return false;
  }
  return true;
}

}  // namespace telemetry
}  // namespace io
}  // namespace rcplane