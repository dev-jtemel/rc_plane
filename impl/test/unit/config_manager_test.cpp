#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "config_manager_mock.hpp"
#include "rcplane/io/config_manager.hpp"

using namespace testing;  // Import the Google Mock namespace

namespace rcplane {
namespace test {

class config_manager_fixture : ::testing::Test {
protected:
  void SetUp() override { std::cout << "setup\n"; }

  void TearDown() override { std::cout << "teardown\n"; }

  config_manager_mock &_config_manager =
      rcplane::io::config_manager::instance();
};

TEST_F(config_manager_fixture, lifecycle) {}

}  // namespace test
}  // namespace rcplane