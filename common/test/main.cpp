#define GTEST
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "rcplane/common/io/config_manager.hpp"
#include "rcplane/common/io/journal.hpp"

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  testing::InitGoogleMock(&argc, argv);
  RCPLANE_LOG_INIT();
  return RUN_ALL_TESTS();
}
