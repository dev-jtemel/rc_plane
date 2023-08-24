#include "rcplane/io/ConfigManager.hpp"

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>  // rm
#include <numeric>
#include <string>

#include "json/json.hpp"

namespace rcplane {
namespace io {

const std::string ConfigManager::k_defaultConfigPath = "configs/config.json";

bool ConfigManager::loadConfig(const std::string &configPath) {
  std::ifstream configStream(configPath);
  if (!configStream.is_open()) {
    std::cerr << "Failed to open config file: " << configPath << std::endl;
    return false;
  }

  // throws nlohmann::json::parse_error.
  m_json = nlohmann::json::parse(configStream);

  // log success
  return true;
}

template<typename T>
T ConfigManager::getValue(const std::string &jsonPath) const {
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

std::string ConfigManager::dumpConfig() const { return m_json.dump(2); }

// Template specializations

template std::string ConfigManager::getValue<std::string>(
    const std::string &) const;
template uint32_t ConfigManager::getValue<uint32_t>(const std::string &) const;
template int8_t ConfigManager::getValue<int8_t>(const std::string &) const;

}  // namespace io
}  // namespace rcplane