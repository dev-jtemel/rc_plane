#ifndef __RCPLANE__IO__CONFIG_MANAGER_HPP__
#define __RCPLANE__IO__CONFIG_MANAGER_HPP__

#include <boost/noncopyable.hpp>
#include <fstream>
#include <string>

#include "json/json.hpp"

namespace rcplane {
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
  static config_manager &instance();

  /**
   * @brief Read the config file.
   */
  void init();

  /**
   * @brief Dump the config file to log.
   */
  void dump();

  /**
   * @brief Get a value from the config manifest.
   * @tparam T The desired type to cast to.
   * @param path The path to the value via dot notation as an rvalue
   * @throws nlohmann::json_cast_error if path is invalid or not of type T
   * @returns Value at path in the manifest casted to type T
   */
  template<typename T>
  T get(const std::string &&path);

private:
  /**
   * @breif Private constructor for singleton.
   * 
   * Set the severity level specified in the config manifest.
   *
   * @throws nlohmann::json::parse_error if manifest is invalid json.
   */
  explicit config_manager();

  /**
   * @brief Read the config manifest to json.
   * @warning Looks at parent path which must be top of the repo!
   */
  void read_config();

  /**
   * @brief Set the severity as specified in the manifest.
   * @param severity the severity in the manfiest as an rvalue.
   */
  void set_log_severity(const std::string &&severity);

  nlohmann::json _config;
};

}  // namespace io
}  // namespace rcplane

#endif  //__RCPLANE__IO__CONFIG_MANAGER_HPP__
