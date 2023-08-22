#include <gtest/gtest.h>

#include "rcplane/common/packet.hpp"

namespace rcplane {
namespace test {

TEST(packet_test, verify_packed) {
  const size_t kSTATE_PACKET_SIZE = 5U;
  const size_t kCONTROL_SURFACE_PACKET_SIZE = 4U;
  const size_t kIMU_PACKET_SIZE = 12U;

  ASSERT_EQ(kSTATE_PACKET_SIZE, sizeof(rcplane::common::state_packet));
  ASSERT_EQ(kCONTROL_SURFACE_PACKET_SIZE,
            sizeof(rcplane::common::control_surface_packet));
  ASSERT_EQ(kIMU_PACKET_SIZE, sizeof(rcplane::common::imu_packet));
}

}  // namespace test
}  // namespace rcplane