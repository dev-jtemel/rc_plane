#ifndef RCPLANE_IO_TELEMETRY_TELEMETRY_MESSAGE_HPP
#define RCPLANE_IO_TELEMETRY_TELEMETRY_MESSAGE_HPP

#include <iostream>
#include <map>
#include <string>

namespace rcplane {
namespace io {
namespace telemetry {
namespace message {

/**
 * @brief Indicates the type of autopilot currently active.
 */
enum class AutopilotType : uint8_t {
  ERROR = 0x00,
  MANUAL_AUTOPILOT = 0x01,
  STABILIZE_AUTOPILOT = 0x02
};

/**
 * @brief GPS fix status.
 */
enum class GpsFixType : uint8_t {
  ERROR = 0x00,
  FIX_1D = 0x01,
  FIX_2D = 0x02,
  FIX_3D = 0x03
};

/**
 * @brief Defines map from enum AutopilotType to string.
 */
extern std::map<AutopilotType, std::string> gAutopilotTypeToStr;

/**
 * @brief Defines map from enum GPSFix to string.
 */
extern std::map<GpsFixType, std::string> gGpsFixTypeToStr;

/**
 * @brief All telemetry messages must derive from this struct and
 * be binary serializable!
 */
struct TelemetryMessage {};

/**
 * @brief Contains all active faults detected.
 */
struct FaultMessage : public TelemetryMessage {
  uint32_t faults{0U};
};

/**
 * @brief Debug information.
 */
struct DebugMessage : public TelemetryMessage {
  uint32_t serialReads{0U};
  uint32_t serialReadTimeouts{0U};
  uint32_t serialWrites{0U};
  uint32_t serialWriteTimeouts{0U};
  uint32_t mainLoopCounter{0U};
};

/**
 * @brief Attitude data.
 */
struct AttitudeMessage : public TelemetryMessage {
  double courseHeading{0.0};
  double rollAngle{0.0};
  double pitchAngle{0.0};
  double yawAngle{0.0};
};

/**
 * @brief Information about the onboard state of the SoM and MCU.
 */
struct OnboardStateMessage : public TelemetryMessage {
  uint32_t mcuTimestamp{0U};
  uint8_t throttle{0U};
  int8_t aileronDeflection{0};
  int8_t elevatorDeflection{0};
  int8_t rudderDeflection{0};
  AutopilotType autopilotType{0U};
  GpsFixType gpsFix{0U};
  uint8_t imuTemperature{0U};
};

inline std::ostream &operator<<(std::ostream &os, const DebugMessage &message) {
  return os << "serialReads: " << message.serialReads
            << " serialReadTimeouts: " << message.serialReadTimeouts
            << " serialWrites: " << message.serialWrites
            << " serialWriteTimeouts: " << message.serialWriteTimeouts
            << " mainLoopCounter: " << message.mainLoopCounter;
}

inline std::ostream &operator<<(std::ostream &os,
                                const AttitudeMessage &message) {
  return os << "courseHeading: " << message.courseHeading
            << " rollAngle: " << message.rollAngle
            << " pitchAngle: " << message.pitchAngle
            << " yawAngle: " << message.yawAngle;
}

inline std::ostream &operator<<(std::ostream &os,
                                const OnboardStateMessage &message) {
  return os << "mcuTimestamp: " << message.mcuTimestamp
            << " throttle: " << +message.throttle
            << " aileronDeflection: " << +message.aileronDeflection
            << " elevatorDeflection: " << +message.elevatorDeflection
            << " rudderDeflection: " << +message.rudderDeflection
            << " autopilot: " << gAutopilotTypeToStr.at(message.autopilotType)
            << " gpsFix: " << gGpsFixTypeToStr.at(message.gpsFix)
            << " imuTemp: " << +message.imuTemperature;
}

}  // namespace message
}  // namespace telemetry
}  // namespace io
}  // namespace rcplane

#endif  //RCPLANE_IO_TELEMETRY_TELEMETRY_MESSAGE_HPP