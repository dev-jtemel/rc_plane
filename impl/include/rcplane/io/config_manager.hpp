/**
 * @file config_manager.hpp
 * @author Jonathon Temelkovski (dev.jtemel@gmail.com)
 * @version 0.1
 * @date 2023-08-22
 */
#ifndef __RCPLANE__IO__CONFIG_MANAGER_HPP__
#define __RCPLANE__IO__CONFIG_MANAGER_HPP__

#include <boost/noncopyable.hpp>
#include <fstream>
#include <string>

#include "json/json.hpp"

#include "rcplane/base_controller.hpp"

namespace rcplane {
namespace io {

/**
 * @brief Manages the distrubtion of config file data.
 *
 * Parse, validate and propagate the configuration JSON data to the rest
 * of the components in the system.
 */
class config_manager : public rcplane::interface::base_controller,
                       public ::boost::noncopyable {
public:
  /**
   * @breif Private constructor for singleton.
   * 
   * Set the severity level specified in the config manifest.
   *
   * @throws nlohmann::json::parse_error if manifest is invalid json.
   */
  explicit config_manager();
  ~config_manager();

  /**
   * @brief Read the config file.
   * @return Initialization status.
   */
  bool init() override;

  /**
   * @brief nop.
   */
  void start() override;

  /**
   * @brief nop.
   */
  void terminate() override;

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

  /**
   * @brief Set the severity as specified in the manifest.
   * @param severity the severity in the manfiest as an rvalue.
   */
  void set_log_severity(const std::string &&severity);

  /**
   * @brief Read the config manifest to json.
   * @warning Looks at parent path which must be top of the repo!
   */
  VIRTUAL_TEST void read_config(const std::string &path);

private:
  const std::string kCONFIG_PATH{"configs/config.json"};
  nlohmann::json _config;
};

}  // namespace io
}  // namespace rcplane

#endif  //__RCPLANE__IO__CONFIG_MANAGER_HPP__
