#ifndef RCPLANE_IO_TELEMETRY_I_TELEMETRY_TRANSMITTER_MQ_HPP
#define RCPLANE_IO_TELEMETRY_I_TELEMETRY_TRANSMITTER_MQ_HPP

#include "rcplane/io/telemetry/TelemetryMessage.hpp"

namespace rcplane {
namespace io {
namespace telemetry {

/**
 * @brief Interface for sending telemetry messages.
 */
class ITelemetryTransmitter {
public:
  /**
   * @brief Initialize the telemetry transmitter.
   * @return True if underlying transport was successfully initialized, false otherwise.
   */
  virtual bool init() = 0;

  /**
   * @brief Send a debug telemetry message.
   * @param message The debug telemetry message to send.
   * @return bool Status of the operation.
   */
  virtual bool sendDebugMessage(const message::DebugMessage &message) = 0;

  /**
   * @brief Send a attitude telemetry message.
   * @param message The attitude telemetry message to send.
   * @return bool Status of the operation.
   */
  virtual bool sendAttitudeMessage(const message::AttitudeMessage &message) = 0;
};

}  // namespace telemetry
}  // namespace io
}  // namespace rcplane

#endif  //RCPLANE_IO_TELEMETRY_I_TELEMETRY_TRANSMITTER_MQ_HPP