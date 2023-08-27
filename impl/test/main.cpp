#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "rcplane/io/Journal.hpp"

int main(int argc, char *argv[]) {
  RCPLANE_LOG_INIT();

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}