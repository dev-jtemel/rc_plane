/**
 * @file config_manager_test.cpp
 * @author Jonathon Temelkovski (dev.jtemel@gmail.com)
 * @version 0.1
 * @date 2023-08-22
 */
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

#include "config_manager_mock.hpp"
#include "rcplane/io/config_manager.hpp"
#include "rcplane/io/journal.hpp"

using namespace testing;  // Import the Google Mock namespace

namespace rcplane {
namespace test {

class config_manager_fixture : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {
    _config_manager_mock->terminate();
    RCPLANE_SEVERITY_UPDATE(trace);
  }

  void load_config(const std::string &path) {
    EXPECT_CALL(*_config_manager_mock, read_config)
        .WillOnce(::testing::Invoke([this, &path]() {
          _config_manager_mock->config_manager::read_config(path);
        }));
    _config_manager_mock->init();
  }

  std::unique_ptr<config_manager_mock> _config_manager_mock =
      std::make_unique<config_manager_mock>();
};

TEST_F(config_manager_fixture, dump) {
  const std::string kREAL_CONFIG = "configs/config.json";
  load_config(kREAL_CONFIG);
  _config_manager_mock->dump();
}

TEST_F(config_manager_fixture, get_dne) {
  const std::string kREAL_CONFIG = "configs/config.json";
  load_config(kREAL_CONFIG);
  EXPECT_THROW(
      {
        try {
          _config_manager_mock->get<std::string>("test.string");

        } catch (const nlohmann::json::type_error &e) {
          EXPECT_STREQ(
              "[json.exception.type_error.304] cannot use at() with null",
              e.what());
          throw;
        }
      },
      nlohmann::json::type_error);
}

TEST_F(config_manager_fixture, get_empty) {
  const std::string kREAL_CONFIG = "configs/config.json";
  load_config(kREAL_CONFIG);
  EXPECT_THROW(
      {
        try {
          _config_manager_mock->get<std::string>("");

        } catch (const nlohmann::json::out_of_range &e) {
          EXPECT_STREQ("[json.exception.out_of_range.403] key '' not found",
                       e.what());
          throw;
        }
      },
      nlohmann::json::out_of_range);
}

TEST_F(config_manager_fixture, get_invalid_st) {
  const std::string kREAL_CONFIG = "configs/config.json";
  load_config(kREAL_CONFIG);
  EXPECT_THROW(
      {
        try {
          _config_manager_mock->get<std::string>("..test");

        } catch (const nlohmann::json::type_error &e) {
          EXPECT_STREQ(
              "[json.exception.type_error.304] cannot use at() with null",
              e.what());
          throw;
        }
      },
      nlohmann::json::type_error);
}

TEST_F(config_manager_fixture, get_invalid_type_str_to_u32) {
  const std::string kTEST_CONFIG = "configs/test_config.json";
  load_config(kTEST_CONFIG);
  EXPECT_THROW(
      {
        try {
          _config_manager_mock->get<uint32_t>("rcplane.test.str");

        } catch (const nlohmann::json::type_error &e) {
          EXPECT_STREQ("[json.exception.type_error.302] type must be number, "
                       "but is string",
                       e.what());
          throw;
        }
      },
      nlohmann::json::type_error);
}

TEST_F(config_manager_fixture, get_invalid_type_str_to_int8) {
  const std::string kTEST_CONFIG = "configs/test_config.json";
  load_config(kTEST_CONFIG);
  EXPECT_THROW(
      {
        try {
          _config_manager_mock->get<int8_t>("rcplane.test.str");

        } catch (const nlohmann::json::type_error &e) {
          EXPECT_STREQ("[json.exception.type_error.302] type must be number, "
                       "but is string",
                       e.what());
          throw;
        }
      },
      nlohmann::json::type_error);
}

TEST_F(config_manager_fixture, get_invalid_type_u32_to_str) {
  const std::string kTEST_CONFIG = "configs/test_config.json";
  load_config(kTEST_CONFIG);
  EXPECT_THROW(
      {
        try {
          _config_manager_mock->get<std::string>("rcplane.test.int8");

        } catch (const nlohmann::json::type_error &e) {
          EXPECT_STREQ("[json.exception.type_error.302] type must be string, "
                       "but is number",
                       e.what());
          throw;
        }
      },
      nlohmann::json::type_error);
}

TEST_F(config_manager_fixture, get_str) {
  const std::string kTEST_CONFIG = "configs/test_config.json";
  load_config(kTEST_CONFIG);

  const std::string ex = "some string";
  const std::string ac =
      _config_manager_mock->get<std::string>("rcplane.test.str");
  ASSERT_EQ(ex, ac);
}

TEST_F(config_manager_fixture, get_int8) {
  const std::string kTEST_CONFIG = "configs/test_config.json";
  load_config(kTEST_CONFIG);

  const int8_t ex = 101;
  const int8_t ac = _config_manager_mock->get<int8_t>("rcplane.test.int8");
  ASSERT_EQ(ex, ac);
}

TEST_F(config_manager_fixture, get_u32) {
  const std::string kTEST_CONFIG = "configs/test_config.json";
  load_config(kTEST_CONFIG);

  const uint32_t ex = 101;
  const uint32_t ac = _config_manager_mock->get<uint32_t>("rcplane.test.int8");
  ASSERT_EQ(ex, ac);
}

TEST_F(config_manager_fixture, set_trace) {
  const std::string kTEST_CONFIG = "configs/trace.json";
  load_config(kTEST_CONFIG);

  RCPLANE_LOG(trace, "test", "trace");
  RCPLANE_LOG(debug, "test", "debug");
  RCPLANE_LOG(info, "test", "info");
  RCPLANE_LOG(warning, "test", "warning");
  RCPLANE_LOG(error, "test", "error");
  RCPLANE_LOG(fatal, "test", "fatal");
}

TEST_F(config_manager_fixture, set_debug) {
  const std::string kTEST_CONFIG = "configs/debug.json";
  load_config(kTEST_CONFIG);
  
  RCPLANE_LOG(trace, "test", "trace");
  RCPLANE_LOG(debug, "test", "debug");
  RCPLANE_LOG(info, "test", "info");
  RCPLANE_LOG(warning, "test", "warning");
  RCPLANE_LOG(error, "test", "error");
  RCPLANE_LOG(fatal, "test", "fatal");
}

TEST_F(config_manager_fixture, set_info) {
  const std::string kTEST_CONFIG = "configs/info.json";
  load_config(kTEST_CONFIG);
  
  RCPLANE_LOG(trace, "test", "trace");
  RCPLANE_LOG(debug, "test", "debug");
  RCPLANE_LOG(info, "test", "info");
  RCPLANE_LOG(warning, "test", "warning");
  RCPLANE_LOG(error, "test", "error");
  RCPLANE_LOG(fatal, "test", "fatal");
}

TEST_F(config_manager_fixture, set_warning) {
  const std::string kTEST_CONFIG = "configs/warning.json";
  load_config(kTEST_CONFIG);
  
  RCPLANE_LOG(trace, "test", "trace");
  RCPLANE_LOG(debug, "test", "debug");
  RCPLANE_LOG(info, "test", "info");
  RCPLANE_LOG(warning, "test", "warning");
  RCPLANE_LOG(error, "test", "error");
  RCPLANE_LOG(fatal, "test", "fatal");
}

TEST_F(config_manager_fixture, set_error) {
  const std::string kTEST_CONFIG = "configs/error.json";
  load_config(kTEST_CONFIG);
  
  RCPLANE_LOG(trace, "test", "trace");
  RCPLANE_LOG(debug, "test", "debug");
  RCPLANE_LOG(info, "test", "info");
  RCPLANE_LOG(warning, "test", "warning");
  RCPLANE_LOG(error, "test", "error");
  RCPLANE_LOG(fatal, "test", "fatal");
}

TEST_F(config_manager_fixture, set_fatal) {
  const std::string kTEST_CONFIG = "configs/fatal.json";
  load_config(kTEST_CONFIG);
  
  RCPLANE_LOG(trace, "test", "trace");
  RCPLANE_LOG(debug, "test", "debug");
  RCPLANE_LOG(info, "test", "info");
  RCPLANE_LOG(warning, "test", "warning");
  RCPLANE_LOG(error, "test", "error");
  RCPLANE_LOG(fatal, "test", "fatal");
}

TEST_F(config_manager_fixture, set_invalid) {
  const std::string kTEST_CONFIG = "configs/invalid.json";
  load_config(kTEST_CONFIG);
  
  RCPLANE_LOG(trace, "test", "trace");
  RCPLANE_LOG(debug, "test", "debug");
  RCPLANE_LOG(info, "test", "info");
  RCPLANE_LOG(warning, "test", "warning");
  RCPLANE_LOG(error, "test", "error");
  RCPLANE_LOG(fatal, "test", "fatal");
}

TEST_F(config_manager_fixture, invalid_json) {
  const std::string kTEST_CONFIG = "configs/bad_json.json";
  EXPECT_THROW({ load_config(kTEST_CONFIG); }, nlohmann::json::parse_error);
  
  RCPLANE_LOG(trace, "test", "trace");
  RCPLANE_LOG(debug, "test", "debug");
  RCPLANE_LOG(info, "test", "info");
  RCPLANE_LOG(warning, "test", "warning");
  RCPLANE_LOG(error, "test", "error");
  RCPLANE_LOG(fatal, "test", "fatal");
}

}  // namespace test
}  // namespace rcplane