#ifndef RCPLANE_IO_TELEMETRY_TELEMETRY_RECEIVER_MQ_HPP
#define RCPLANE_IO_TELEMETRY_TELEMETRY_RECEIVER_MQ_HPP

#include <boost/interprocess/ipc/message_queue.hpp>

#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/telemetry/TelemetryMessage.hpp"

namespace rcplane {
namespace io {
namespace telemetry {

class TelemetryReceiverMQ {
public:
  TelemetryReceiverMQ(const ConfigManager &configManager);
  ~TelemetryReceiverMQ();

  virtual bool init();
  virtual bool receiveDebugMessage();

private:
  // TODO: config
  std::string c_mQueueName = "rcplane_telemetry_mq";
  std::unique_ptr<boost::interprocess::message_queue> m_mQueue;
};

}  // namespace telemetry
}  // namespace io
}  // namespace rcplane

#endif  //RCPLANE_IO_TELEMETRY_TELEMETRY_RECEIVER_MQ_HPP