#include <cassert>
#include <iostream>
#include "rcplane/common/io/packet.hpp"

using namespace rcplane::common::io;
using type = packet<int, int>;

void uint8_to_uint8() {
  packet<uint8_t, uint8_t> p(packet_type::state); 
  assert(p.type() == packet_type::state);

  uint8_t test_value = 0;
  uint8_t expected_value = 0;

  p.set(test_value);

  assert(p.raw() == test_value);
  assert(p.data() == expected_value);

  test_value = 255;
  expected_value = 255;

  p.set(test_value);

  assert(p.raw() == test_value);
  assert(p.data() == expected_value);
}

void uint8_to_int8() {
  packet<uint8_t, int8_t> p(packet_type::aileron); 
  assert(p.type() == packet_type::aileron);

  uint8_t test_value = 0;
  int8_t expected_value = 0;

  p.set(test_value);

  assert(p.raw() == test_value);
  assert(p.data() == expected_value);

  test_value = 255;
  expected_value = -1;

  p.set(test_value);

  assert(p.raw() == test_value);
  assert(p.data() == expected_value);

  test_value = 129;
  expected_value = -127;

  p.set(test_value);

  assert(p.raw() == test_value);
  assert(p.data() == expected_value);

  test_value = 20;
  expected_value = 20;

  p.set(test_value);

  assert(p.raw() == test_value);
  assert(p.data() == expected_value);
}

void uint32_to_float() {
  packet<uint32_t, float> p(packet_type::pitch); 
  assert(p.type() == packet_type::pitch);

  uint32_t test_value = 0;
  float expected_value = 0;

  p.set(test_value);

  assert(p.raw() == test_value);
  assert(p.data() == expected_value);

  test_value = 0b01000001001000011001100110011010;
  expected_value = 10.1;

  p.set(test_value);

  assert(p.raw() == test_value);
  assert(p.data() == expected_value);

  test_value = 0b11000001001000011001100110011010;
  expected_value = -10.1;

  p.set(test_value);

  assert(p.raw() == test_value);
  assert(p.data() == expected_value);
}

int main(int argc, char *argv[]) {
  uint8_to_uint8();
  uint8_to_int8();
  uint32_to_float();
}
