#ifndef RCPLANE_IO_TELEMETRY_TELEMETRY_TRANSMITTER_MQ_HPP
#define RCPLANE_IO_TELEMETRY_TELEMETRY_TRANSMITTER_MQ_HPP

#include <boost/interprocess/ipc/message_queue.hpp>

#include "rcplane/io/Journal.hpp"
#include "rcplane/io/telemetry/TelemetryMessage.hpp"

namespace rcplane {
namespace io {
namespace telemetry {

class TelemetryTransmitterMQ {
public:
  TelemetryTransmitterMQ() { RCPLANE_LOG_METHOD(); }
  ~TelemetryTransmitterMQ() {
    RCPLANE_LOG_METHOD();
    boost::interprocess::message_queue::remove(c_mQueueName.c_str());
  }

  virtual bool init() {
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

  virtual bool sendDebugMessage(const message::DebugMessage &message) {
    RCPLANE_LOG_METHOD();
    m_mQueue->send((uint8_t *)&message, sizeof(message), 0);
    return true;
  }

private:
  std::string c_mQueueName = "rcplane_telemetry_mq";
  uint32_t c_mQueueSize = 1024U;
  std::unique_ptr<boost::interprocess::message_queue> m_mQueue;
};

}  // namespace telemetry
}  // namespace io
}  // namespace rcplane

#endif  //RCPLANE_IO_TELEMETRY_TELEMETRY_TRANSMITTER_MQ_HPP