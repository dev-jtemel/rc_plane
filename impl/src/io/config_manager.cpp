/**
 * @file config_manager.cpp
 * @author Jonathon Temelkovski (dev.jtemel@gmail.com)
 * @version 0.1
 * @date 2023-08-22
 */
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

config_manager::config_manager()
  : rcplane::interface::base_controller("config_manager") {
  RCPLANE_ENTER();
}

bool config_manager::init() {
  RCPLANE_ENTER();

  read_config();

  set_log_severity(
      _config["rcplane"]["io"]["journal"]["severity"].get<std::string>());
  RCPLANE_LOG(info, _tag, "initialized");
  return true;
}

void config_manager::start() {
  RCPLANE_ENTER();
  RCPLANE_LOG(info, _tag, "started");
}

void config_manager::terminate() {
  RCPLANE_ENTER();
  RCPLANE_LOG(info, _tag, "started");
}

void config_manager::dump() {
  RCPLANE_ENTER();

  RCPLANE_LOG(trace, "config_manager", "\n" << _config.dump(2));
}

template<typename T>
T config_manager::get(const std::string &&path) {
  RCPLANE_ENTER();

  std::vector<std::string> split_paths;
  boost::split(split_paths, path, boost::is_any_of("."));

  auto sub_config = _config;
  std::for_each(split_paths.begin(),
                split_paths.end() - 1,
                [&sub_config](auto &s) { sub_config = sub_config[s]; });

  return sub_config.at(split_paths[split_paths.size() - 1]).get<T>();
}

void config_manager::read_config() {
  RCPLANE_ENTER();

  std::ifstream f("config.json");
  _config = nlohmann::json::parse(f);
}

void config_manager::set_log_severity(const std::string &&severity) {
  RCPLANE_ENTER();

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
