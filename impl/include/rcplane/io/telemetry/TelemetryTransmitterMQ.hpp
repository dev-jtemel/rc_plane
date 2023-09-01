#ifndef RCPLANE_IO_TELEMETRY_TELEMETRY_TRANSMITTER_MQ_HPP
#define RCPLANE_IO_TELEMETRY_TELEMETRY_TRANSMITTER_MQ_HPP

#include <boost/interprocess/ipc/message_queue.hpp>

#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/telemetry/TelemetryMessage.hpp"

namespace rcplane {
namespace io {
namespace telemetry {

class TelemetryTransmitterMQ {
public:
  TelemetryTransmitterMQ(const ConfigManager &configManager);
  ~TelemetryTransmitterMQ();

  virtual bool init();
  virtual bool sendDebugMessage(const message::DebugMessage &message);

private:
  // TODO: config
  std::string c_mQueueName = "rcplane_telemetry_mq";
  uint32_t c_mQueueSize = 10U;
  std::unique_ptr<boost::interprocess::message_queue> m_mQueue;
};

}  // namespace telemetry
}  // namespace io
}  // namespace rcplane

#endif  //RCPLANE_IO_TELEMETRY_TELEMETRY_TRANSMITTER_MQ_HPP