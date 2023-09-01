#ifndef RCPLANE_IO_TELEMETRY_TELEMETRY_RECEIVER_MQ_HPP
#define RCPLANE_IO_TELEMETRY_TELEMETRY_RECEIVER_MQ_HPP

#include <boost/interprocess/ipc/message_queue.hpp>

#include "rcplane/io/Journal.hpp"
#include "rcplane/io/telemetry/TelemetryMessage.hpp"

namespace rcplane {
namespace io {
namespace telemetry {

class TelemetryReceiverMQ {
public:
  TelemetryReceiverMQ() { RCPLANE_LOG_METHOD(); }
  ~TelemetryReceiverMQ() { RCPLANE_LOG_METHOD(); }

  virtual bool init() {
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

  virtual bool receiveMessage() {
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

private:
  std::string c_mQueueName = "rcplane_telemetry_mq";
  std::unique_ptr<boost::interprocess::message_queue> m_mQueue;
};

}  // namespace telemetry
}  // namespace io
}  // namespace rcplane

#endif  //RCPLANE_IO_TELEMETRY_TELEMETRY_RECEIVER_MQ_HPP