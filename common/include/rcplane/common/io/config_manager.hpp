#ifndef __RCPLANE__COMMON__IO__CONFIG_MANAGER_HPP__
#define __RCPLANE__COMMON__IO__CONFIG_MANAGER_HPP__

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "rcplane/common/io/journal.hpp"

#include "json/json.hpp"

namespace rcplane {
namespace common {
namespace io {

/**
 * @brief Singleton that manages the distrubtion of config file data.
 *
 * Parse, validate and propagate the configuration JSON data to the rest
 * of the components in the system.
 *
 * @warning This is a singleton class.
 */
class config_manager {
public:
  static config_manager &instance() {
    static config_manager c;
    return c;
  }

  template<typename T>
  T get(std::string path) {
    std::vector<std::string> split_paths;
    boost::split(split_paths, path, boost::is_any_of("."));

    auto sub_config = _config;
    std::for_each(split_paths.begin(),
                  split_paths.end() - 1,
                  [&sub_config](auto &s) { sub_config = sub_config[s]; });

    return sub_config.at(split_paths[split_paths.size() - 1]).get<T>();
  }

private:
  explicit config_manager() {
    read_config();

    set_log_severity(_config["common"]["log"]["level"].get<std::string>());

    RCPLANE_LOG(trace, "config_manager", "\n" << _config.dump(2));
  }

  void read_config() {
    std::ifstream f("/home/jtemel/ws/rc_plane/config.json");
    _config = nlohmann::json::parse(f);
  }

  void set_log_severity(std::string severity) {
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

  nlohmann::json _config;
};

}  // namespace io
}  // namespace common
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__IO__CONFIG_MANAGER_HPP__
