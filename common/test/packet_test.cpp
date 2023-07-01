#include "rcplane/common/io/packet.hpp"
#include <gtest/gtest.h>

using namespace rcplane::common::io;
using type = packet<int, int>;

TEST(packet_test, uint8_to_uint8_zero) {
  packet<uint8_t, uint8_t> p(packet_type::state);
  ASSERT_EQ(p.type(), packet_type::state);

  uint8_t test_value = 0;
  uint8_t expected_value = 0;

  p.set(test_value);

  ASSERT_EQ(p.raw(), test_value);
  ASSERT_EQ(p.data(), expected_value);
}

TEST(packet_test, uint8_to_uint8_max) {
  packet<uint8_t, uint8_t> p(packet_type::state);
  ASSERT_EQ(p.type(), packet_type::state);

  uint8_t test_value = 255;
  uint8_t expected_value = 255;

  p.set(test_value);

  ASSERT_EQ(p.raw(), test_value);
  ASSERT_EQ(p.data(), expected_value);
}

TEST(packet_test, uint8_to_int8_zero) {
  packet<uint8_t, int8_t> p(packet_type::aileron);

  uint8_t test_value = 0;
  int8_t expected_value = 0;

  p.set(test_value);

  ASSERT_EQ(p.raw(), test_value);
  ASSERT_EQ(p.data(), expected_value);
}

TEST(packet_test, uint8_to_int8_max) {
  packet<uint8_t, int8_t> p(packet_type::aileron);

  uint8_t test_value = 255;
  int8_t expected_value = -1;

  p.set(test_value);

  ASSERT_EQ(p.raw(), test_value);
  ASSERT_EQ(p.data(), expected_value);
}

TEST(packet_test, uint8_to_int8_mid_flip) {
  packet<uint8_t, int8_t> p(packet_type::aileron);

  uint8_t test_value = 129;
  int8_t expected_value = -127;

  p.set(test_value);

  ASSERT_EQ(p.raw(), test_value);
  ASSERT_EQ(p.data(), expected_value);
}

TEST(packet_test, uint8_to_int8_mid_pos) {
  packet<uint8_t, int8_t> p(packet_type::aileron);

  uint8_t test_value = 20;
  int8_t expected_value = 20;

  p.set(test_value);

  ASSERT_EQ(p.raw(), test_value);
  ASSERT_EQ(p.data(), expected_value);
}

TEST(packet_test, uint32_to_float_zero) {
  packet<uint32_t, float> p(packet_type::pitch);

  uint32_t test_value = 0;
  float expected_value = 0;

  p.set(test_value);

  ASSERT_EQ(p.raw(), test_value);
  ASSERT_EQ(p.data(), expected_value);
}

TEST(packet_test, uint32_to_float_pos) {
  packet<uint32_t, float> p(packet_type::pitch);

  uint32_t test_value = 0b01000001001000011001100110011010;
  float expected_value = 10.1;

  p.set(test_value);

  ASSERT_EQ(p.raw(), test_value);
  ASSERT_EQ(p.data(), expected_value);
}

TEST(packet_test, uint32_to_float_neg) {
  packet<uint32_t, float> p(packet_type::pitch);

  uint32_t test_value = 0b11000001001000011001100110011010;
  float expected_value = -10.1;

  p.set(test_value);

  ASSERT_EQ(p.raw(), test_value);
  ASSERT_EQ(p.data(), expected_value);
}
