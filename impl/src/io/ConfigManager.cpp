#include "rcplane/io/ConfigManager.hpp"

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <numeric>
#include <string>

#include "json/json.hpp"

#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace io {

const std::string ConfigManager::k_defaultConfigPath = "configs/config.json";

ConfigManager::ConfigManager() { RCPLANE_LOG_METHOD(); }

ConfigManager::~ConfigManager() { RCPLANE_LOG_METHOD(); }

bool ConfigManager::loadConfig(const std::string &configPath) {
  RCPLANE_LOG_METHOD();

  std::ifstream configStream(configPath);
  if (!configStream.is_open()) {
    RCPLANE_LOG(error, "Failed to open config file: " << configPath);
    return false;
  }

  // throws nlohmann::json::parse_error.
  m_json = nlohmann::json::parse(configStream);

  m_configLoaded = true;
  return true;
}

template<typename T>
T ConfigManager::getValue(const std::string &jsonPath) const {
  RCPLANE_LOG_METHOD();

  std::vector<std::string> splitPaths;
  boost::split(splitPaths, jsonPath, boost::is_any_of("."));

  nlohmann::json subJson =
      std::accumulate(splitPaths.begin(),
                      splitPaths.end(),
                      m_json,
                      [](nlohmann::json &subJson, const std::string &path) {
                        return subJson[path];
                      });

  return subJson.get<T>();
}

std::string ConfigManager::dumpConfig() const {
  RCPLANE_LOG_METHOD();
  return m_json.dump(2);
}

bool ConfigManager::isConfigLoaded() const {
  RCPLANE_LOG_METHOD();
  return m_configLoaded;
}

// Template specializations

template std::string ConfigManager::getValue<std::string>(
    const std::string &) const;
template uint32_t ConfigManager::getValue<uint32_t>(const std::string &) const;
template int8_t ConfigManager::getValue<int8_t>(const std::string &) const;
template uint8_t ConfigManager::getValue<uint8_t>(const std::string &) const;
template double ConfigManager::getValue<double>(const std::string &) const;

}  // namespace io
}  // namespace rcplane