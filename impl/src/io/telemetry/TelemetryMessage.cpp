#include "rcplane/io/telemetry/TelemetryMessage.hpp"

#include <map>
#include <string>

namespace rcplane {
namespace io {
namespace telemetry {
namespace message {

/**
 * @brief Defines map from enum AutopilotType to string.
 */
std::map<AutopilotType, std::string> gAutopilotTypeToStr = {
    {rcplane::io::telemetry::message::AutopilotType::ERROR, "ERROR"},
    {rcplane::io::telemetry::message::AutopilotType::MANUAL_AUTOPILOT,
     "MANUAL_AUTOPILOT"},
    {rcplane::io::telemetry::message::AutopilotType::STABILIZE_AUTOPILOT,
     "STABILIZE_AUTOPILOT"}};

}  // namespace message
}  // namespace telemetry
}  // namespace io
}  // namespace rcplane