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
    {rcplane::io::telemetry::message::AutopilotType::ERROR, "ERR"},
    {rcplane::io::telemetry::message::AutopilotType::MANUAL_AUTOPILOT, "MAN"},
    {rcplane::io::telemetry::message::AutopilotType::STABILIZE_AUTOPILOT,
     "STL"}};

}  // namespace message
}  // namespace telemetry
}  // namespace io
}  // namespace rcplane