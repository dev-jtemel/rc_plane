#include "rcplane/io/ConfigManager.hpp"

#include <gtest/gtest.h>
#include <memory>

namespace rcplane {
namespace test {

class ConfigManagerFixture : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {
    m_configManager.reset();
  }

  std::unique_ptr<io::ConfigManager> m_configManager =
      std::make_unique<io::ConfigManager>();
};

TEST_F(ConfigManagerFixture, loadConfig_fileDne) {
  const std::string kConfigFile = "/tmp/does-not-exist.json";
  ASSERT_FALSE(m_configManager->loadConfig(kConfigFile));
}

TEST_F(ConfigManagerFixture, loadConfig_fileInvalidJson) {
  const std::string kConfigFile = "configs/test/unit/invalidJson.json";
  EXPECT_THROW(m_configManager->loadConfig(kConfigFile), nlohmann::json::parse_error);
}

TEST_F(ConfigManagerFixture, loadConfig_success) {
  const std::string kConfigFile = "configs/test/unit/configManager.json";
  ASSERT_TRUE(m_configManager->loadConfig(kConfigFile));
}

TEST_F(ConfigManagerFixture, getValue_emptyPath) {
  const std::string kConfigFile = "configs/test/unit/configManager.json";
  ASSERT_TRUE(m_configManager->loadConfig(kConfigFile));

  EXPECT_THROW(m_configManager->getValue<std::string>(""), nlohmann::json::type_error);
}

TEST_F(ConfigManagerFixture, getValue_invalidPath) {
  const std::string kConfigFile = "configs/test/unit/configManager.json";
  ASSERT_TRUE(m_configManager->loadConfig(kConfigFile));

  EXPECT_THROW(m_configManager->getValue<std::string>("rcplane..bad"), nlohmann::json::type_error);
}

TEST_F(ConfigManagerFixture, getValue_nonexistentPath) {
  const std::string kConfigFile = "configs/test/unit/configManager.json";
  ASSERT_TRUE(m_configManager->loadConfig(kConfigFile));

  EXPECT_THROW(m_configManager->getValue<std::string>("rcplane.test.dne"), nlohmann::json::type_error);
}

TEST_F(ConfigManagerFixture, getValue_strToInt) {
  const std::string kConfigFile = "configs/test/unit/configManager.json";
  ASSERT_TRUE(m_configManager->loadConfig(kConfigFile));

  EXPECT_THROW(m_configManager->getValue<int8_t>("rcplane.test.string"), nlohmann::json::type_error);
}

TEST_F(ConfigManagerFixture, getValue_intToStr) {
  const std::string kConfigFile = "configs/test/unit/configManager.json";
  ASSERT_TRUE(m_configManager->loadConfig(kConfigFile));

  EXPECT_THROW(m_configManager->getValue<std::string>("rcplane.test.int"), nlohmann::json::type_error);
}

TEST_F(ConfigManagerFixture, getValue_str) {
  const std::string kConfigFile = "configs/test/unit/configManager.json";
  ASSERT_TRUE(m_configManager->loadConfig(kConfigFile));

  const std::string cActualValue = m_configManager->getValue<std::string>("rcplane.test.string");
  const std::string kExpectedValue = "test string";

  ASSERT_STREQ(cActualValue.c_str(), kExpectedValue.c_str());
}

TEST_F(ConfigManagerFixture, getValue_uint32) {
  const std::string kConfigFile = "configs/test/unit/configManager.json";
  ASSERT_TRUE(m_configManager->loadConfig(kConfigFile));

  const uint32_t cActualValue = m_configManager->getValue<uint32_t>("rcplane.test.uint");
  const uint32_t kExpectedValue = 100;

  ASSERT_EQ(cActualValue, kExpectedValue);
}

TEST_F(ConfigManagerFixture, getValue_int8) {
  const std::string kConfigFile = "configs/test/unit/configManager.json";
  ASSERT_TRUE(m_configManager->loadConfig(kConfigFile));

  const int8_t cActualValue = m_configManager->getValue<int8_t>("rcplane.test.uint");
  const int8_t kExpectedValue = 100;

  ASSERT_EQ(cActualValue, kExpectedValue);
}

TEST_F(ConfigManagerFixture, getValue_int8) {
  const std::string kConfigFile = "configs/test/unit/configManager.json";
  ASSERT_TRUE(m_configManager->loadConfig(kConfigFile));

  const int8_t cActualValue = m_configManager->getValue<int8_t>("rcplane.test.int");
  const int8_t kExpectedValue = -100;

  ASSERT_EQ(cActualValue, kExpectedValue);
}

}  // namespace test
}  // namespace rcplane