#ifndef RCPLANE_TEST_PACKET_TEST_UTILS_HPP
#define RCPLANE_TEST_PACKET_TEST_UTILS_HPP

#include "rcplane/common/Packet.hpp"
#include <random>

namespace rcplane {
namespace test {
namespace util {

template<typename T>
inline T getRandomValue() {
  static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");

  static std::random_device rd;
  static std::mt19937 gen(rd());

  if constexpr (std::is_integral_v<T>) {
    std::uniform_int_distribution<T> distribution(
        std::numeric_limits<T>::min(),
        std::numeric_limits<T>::max());
    return distribution(gen);
  } else {
    std::uniform_real_distribution<T> distribution(
        std::numeric_limits<T>::min(),
        std::numeric_limits<T>::max());
    return distribution(gen);
  }
}

template<typename T>
inline T createPacket() {
  if constexpr (std::is_same_v<T, common::HandshakePacket>) {
    return {getRandomValue<uint8_t>()};
  } else if constexpr (std::is_same_v<T, common::StatePacket>) {
    return {getRandomValue<uint32_t>(), getRandomValue<uint8_t>()};
  } else if constexpr (std::is_same_v<T, common::ControlSurfacePacket>) {
    return {getRandomValue<uint8_t>(),
            getRandomValue<int8_t>(),
            getRandomValue<int8_t>(),
            getRandomValue<int8_t>()};
  } else if constexpr (std::is_same_v<T, common::ImuPacket>) {
    return {getRandomValue<float>(),
            getRandomValue<float>(),
            getRandomValue<float>(),
            getRandomValue<float>(),
            getRandomValue<float>(),
            getRandomValue<float>(),
            getRandomValue<float>()};
  }
  return {};
}

}  // namespace util
}  // namespace test
}  // namespace rcplane

#endif  // RCPLANE_TEST_PACKET_TEST_UTILS_HPP