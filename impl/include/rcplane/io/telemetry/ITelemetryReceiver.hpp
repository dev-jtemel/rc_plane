#ifndef RCPLANE_IO_TELEMETRY_I_TELEMETRY_RECEIVER_MQ_HPP
#define RCPLANE_IO_TELEMETRY_I_TELEMETRY_RECEIVER_MQ_HPP

#include "rcplane/io/telemetry/TelemetryMessage.hpp"

namespace rcplane {
namespace io {
namespace telemetry {

/**
 * @brief Interface for receiving telemetry messages.
 */
class ITelemetryReceiver {
public:
  /**
   * @brief Initialize the telemetry receiver.
   * @return True if transport layer is initialized successfully, false otherwise.
   */
  virtual bool init() = 0;

  /**
   * @brief Receive a debug telemetry message.
   * @return bool Status of the read operation.
   */
  virtual bool receiveDebugMessage() = 0;
};

}  // namespace telemetry
}  // namespace io
}  // namespace rcplane

#endif  //RCPLANE_IO_TELEMETRY_I_TELEMETRY_RECEIVER_MQ_HPP