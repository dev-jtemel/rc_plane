#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/noncopyable.hpp>
#include <fstream>
#include <string>
#include <vector>

#include "json/json.hpp"

#include "rcplane/io/config_manager.hpp"
#include "rcplane/io/journal.hpp"

namespace rcplane {
namespace io {

config_manager &config_manager::instance() {
  static config_manager c;
  return c;
}

void config_manager::init() {
  read_config();

  set_log_severity(
      _config["rcplane"]["io"]["journal"]["severity"].get<std::string>());
}

void config_manager::dump() {
  RCPLANE_LOG(trace, "config_manager", "\n" << _config.dump(2));
}

template<typename T>
T config_manager::get(const std::string &&path) {
  std::vector<std::string> splitPaths;
  boost::split(splitPaths, path, boost::is_any_of("."));

  nlohmann::json subConfig = _config;
  for (const auto &pathSegment : splitPaths) {
    subConfig = subConfig[pathSegment];
  }

  return subConfig.get<T>();
}

config_manager::config_manager() {}

void config_manager::read_config() {
  std::ifstream f("config.json");
  _config = nlohmann::json::parse(f);
}

void config_manager::set_log_severity(const std::string &&severity) {
  if (severity == "trace") {
    RCPLANE_SEVERITY_UPDATE(trace);
  } else if (severity == "debug") {
    RCPLANE_SEVERITY_UPDATE(debug);
  } else if (severity == "info") {
    RCPLANE_SEVERITY_UPDATE(info);
  } else if (severity == "warning") {
    RCPLANE_SEVERITY_UPDATE(warning);
  } else if (severity == "error") {
    RCPLANE_SEVERITY_UPDATE(error);
  } else if (severity == "fatal") {
    RCPLANE_SEVERITY_UPDATE(fatal);
  } else {
    RCPLANE_LOG(error,
                "config_manager",
                "invalid logging severity :: " << severity);
  }
}

template std::string config_manager::get<std::string>(const std::string &&);
template uint32_t config_manager::get<uint32_t>(const std::string &&);
template int8_t config_manager::get<int8_t>(const std::string &&);

}  // namespace io
}  // namespace rcplane
