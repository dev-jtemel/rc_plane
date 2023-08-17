#ifndef __RCPLANE__COMMON__IO__CONFIG_MANAGER_HPP__
#define __RCPLANE__COMMON__IO__CONFIG_MANAGER_HPP__

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/noncopyable.hpp>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "json/json.hpp"

#include "rcplane/common/io/journal.hpp"

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
class config_manager : public ::boost::noncopyable {
public:
  /**
   * @brief Retreive the instance or create one if required.
   */
  static config_manager &instance() {
    static config_manager c;
    return c;
  }

  /**
   * @brief Get a value from the config manifest.
   * @param path The path to the value via dot notation as an rvalue
   * @throws nlohmann::json_cast_error if path is invalid or not of type T
   * @returns Value at path in the manifest casted to type T
   */
  template<typename T>
  T get(const std::string &&path) {
    std::vector<std::string> split_paths;
    boost::split(split_paths, path, boost::is_any_of("."));

    auto sub_config = _config;
    std::for_each(split_paths.begin(),
                  split_paths.end() - 1,
                  [&sub_config](auto &s) { sub_config = sub_config[s]; });

    return sub_config.at(split_paths[split_paths.size() - 1]).get<T>();
  }

private:
  /**
   * @breif Private constructor for singleton.
   * 
   * Set the severity level specified in the config manifest.
   *
   * @throws nlohmann::json::parse_error if manifest is invalid json.
   */
  explicit config_manager() {
    read_config();

    set_log_severity(
        _config["common"]["io"]["journal"]["severity"].get<std::string>());

    RCPLANE_LOG(trace, "config_manager", "\n" << _config.dump(2));
  }

  /**
   * @brief Read the config manifest to json.
   * @warning Looks at parent path which must be top of the repo!
   */
  void read_config() {
    std::ifstream f("config.json");
    _config = nlohmann::json::parse(f);
  }

  /**
   * @brief Set the severity as specified in the manifest.
   * @param severity the severity in the manfiest as an rvalue.
   */
  void set_log_severity(const std::string &&severity) {
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
