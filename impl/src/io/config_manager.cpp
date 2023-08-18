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

template<typename T>
T config_manager::get(const std::string &&path) {
  std::vector<std::string> split_paths;
  boost::split(split_paths, path, boost::is_any_of("."));

  auto sub_config = _config;
  std::for_each(split_paths.begin(),
                split_paths.end() - 1,
                [&sub_config](auto &s) { sub_config = sub_config[s]; });

  return sub_config.at(split_paths[split_paths.size() - 1]).get<T>();
}

config_manager::config_manager() {
  read_config();

  set_log_severity(
      _config["common"]["io"]["journal"]["severity"].get<std::string>());

  RCPLANE_LOG(trace, "config_manager", "\n" << _config.dump(2));
}

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
