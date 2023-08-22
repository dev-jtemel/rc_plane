#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "rcplane/io/journal.hpp"

int main(int argc, char **argv) {
  RCPLANE_LOG_INIT_TEST();
  
  testing::InitGoogleTest(&argc, argv);
  testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}