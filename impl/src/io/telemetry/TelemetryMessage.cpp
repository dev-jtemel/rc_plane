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
     "SBL"}};

/**
 * @brief Defines map from enum GpsFixType to string.
 */
std::map<GpsFixType, std::string> gGpsFixTypeToStr = {
    {rcplane::io::telemetry::message::GpsFixType::ERROR, "ERR"},
    {rcplane::io::telemetry::message::GpsFixType::FIX_1D, "F1D"},
    {rcplane::io::telemetry::message::GpsFixType::FIX_2D, "F2D"},
    {rcplane::io::telemetry::message::GpsFixType::FIX_3D, "F3D"}};

}  // namespace message
}  // namespace telemetry
}  // namespace io
}  // namespace rcplane