#ifndef RCPLANE_IO_CONFIG_MANAGER_HPP_
#define RCPLANE_IO_CONFIG_MANAGER_HPP_

#include <boost/noncopyable.hpp>
#include <string>

#include "json/json.hpp"

namespace rcplane {
namespace io {

/**
 * @brief Maintain and distribute the config file data.
 *
 * Parse, validate and propagate the configuration JSON data to the rest
 * of the components in the system.
 */
class ConfigManager : public ::boost::noncopyable {
public:
  ConfigManager() = default;
  ~ConfigManager() = default;

  /**
   * @brief Load the JSON configuration file.
   * @param configPath The path to the JSON configuration file.
   * Default value is k_defaultConfigPath.
   * @throws nlohmann::json::parse_error if the configuration file is not valid.
   * @return true if the configuration file is read and contains valid JSON.
   * @return false otherwise.
   */
  bool loadConfig(const std::string &configPath = k_defaultConfigPath);

  /**
   * @brief Get the value at jsonPath as type T from the configuration file.
   * @tparam T The type to cast the value to.
   * @param jsonPath The "." separated path to the value.
   * @throws nlohmann::type_error if the value at jsonPath is not of type T.
   * @throws nlohmann::out_of_range if the value at jsonPath does not exist.
   * @return T The value at jsonPath as type T.
   */
  template<typename T>
  T getValue(const std::string &jsonPath) const;

  /**
   * @brief Dump the JSON configuration file to the log.
   * @return The JSON configuration file as a string (indent: 2 spaces).
   */
  std::string dumpConfig() const;

private:
  static const std::string k_defaultConfigPath;
  nlohmann::json m_json{};
};

}  // namespace io
}  // namespace rcplane

#endif  //RCPLANE_IO_CONFIG_MANAGER_HPP_