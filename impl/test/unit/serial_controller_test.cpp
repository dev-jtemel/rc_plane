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
#include "rcplane/io/journal.hpp"
#include "rcplane/io/serial_controller.hpp"
#include "serial_controller_mock.hpp"

using namespace testing;  // Import the Google Mock namespace

namespace rcplane {
namespace test {

class serial_controller_fixture : public ::testing::Test {
protected:
  void SetUp() override {
    EXPECT_CALL(_config_manager_mock, read_config)
        .WillOnce(::testing::Invoke([this]() {
          _config_manager_mock.config_manager::read_config(kSERIAL_TEST_CONFIG);
        }));
    _config_manager_mock.init();
    _serial_controller_mock =
        std::make_unique<serial_controller_mock>(_config_manager_mock,
                                                 _mock_io);

    ASSERT_TRUE(_serial_controller_mock->init());
    _serial_controller_mock->start();
  }

  void TearDown() override {
    _serial_controller_mock->terminate();
    RCPLANE_SEVERITY_UPDATE(trace);
  }

  const std::string kSERIAL_TEST_CONFIG{
      "configs/test/unit/serial_controller.json"};
  boost::asio::io_context _mock_io;
  config_manager_mock _config_manager_mock;
  std::unique_ptr<serial_controller_mock> _serial_controller_mock;
};

TEST_F(serial_controller_fixture, dump) {}

}  // namespace test
}  // namespace rcplane