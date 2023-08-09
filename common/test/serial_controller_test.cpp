#include "rcplane/common/io/serial_controller.hpp"
#include <bitset>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace rcplane::common::io;

class serial_controller_mock : public serial_controller {
public:
  MOCK_METHOD(bool, p_open_port, (), (override));
  MOCK_METHOD(boost::optional<uint64_t>, p_read_line, (), (override));
  MOCK_METHOD(bool, p_handshake_mcu, (), (override));
};

TEST(serial_controller_test, lifecycle) {
  serial_controller_mock sc;
  EXPECT_CALL(sc, p_open_port)
      .WillOnce(::testing::Invoke(::testing::Return(true)));
  EXPECT_CALL(sc, p_handshake_mcu)
      .WillOnce(::testing::Invoke(::testing::Return(true)));

  sc.init();
  sc.start();
  sc.terminate();
}

TEST(serial_controller_test, read_buffer_once) {
  const uint8_t estate = 0x11;
  const uint8_t emotor = 0x44;
  const int8_t eaileron = 0x78;
  const int8_t eelevator = 0x7A;
  const int8_t erudder = 0x7F;
  const uint64_t packet = estate | emotor << 8 | eaileron << 16
      | eelevator << 24 | (uint64_t)erudder << 32;

  serial_controller_mock sc;
  EXPECT_CALL(sc, p_open_port)
      .WillOnce(::testing::Invoke(::testing::Return(true)));
  EXPECT_CALL(sc, p_handshake_mcu)
      .WillOnce(::testing::Invoke(::testing::Return(true)));
  EXPECT_CALL(sc, p_read_line)
      .WillOnce(::testing::Invoke(
          ::testing::Return<boost::optional<uint64_t>>({packet})))
      .WillRepeatedly(
          ::testing::Invoke(::testing::Return<boost::optional<uint64_t>>({})));

  sc.init();
  sc.start();

  auto cb = [=](uint8_t rstate,
                uint8_t rmotor,
                int8_t raileron,
                int8_t relevator,
                int8_t rrudder) -> void {
    RCPLANE_ENTER();

    ASSERT_EQ(estate, rstate);
    ASSERT_EQ(emotor, rmotor);
    ASSERT_EQ(eaileron, raileron);
    ASSERT_EQ(eelevator, relevator);
    ASSERT_EQ(erudder, rrudder);
  };

  sc.register_cs_cb(cb);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  sc.terminate();
}

TEST(serial_controller_test, read_buffer_full_set) {
  const uint8_t estate = 0x1F;
  const uint8_t emotor = 0x4F;
  const int8_t eaileron = 0x7F;
  const int8_t eelevator = 0x7F;
  const int8_t erudder = 0x7F;

  const uint32_t gyro_x = 0b01000001001000011001100110011010;
  const uint32_t gyro_y = 0b11000001001000011001100110011010;
  const uint32_t gyro_z = 0b01000001001000011001100110011010;

  const float egyro_x = 10.1f;
  const float egyro_y = -10.1f;
  const float egyro_z = 10.1f;

  const uint64_t cs_packet = estate | emotor << 8 | eaileron << 16
      | eelevator << 24 | (uint64_t)erudder << 32;
  const uint64_t gyro_packet_1 = (uint64_t)gyro_x << 32 | gyro_y;
  const uint64_t gyro_packet_2 = (uint64_t)gyro_z << 32;

  serial_controller_mock sc;
  EXPECT_CALL(sc, p_open_port)
      .WillOnce(::testing::Invoke(::testing::Return(true)));
  EXPECT_CALL(sc, p_handshake_mcu)
      .WillOnce(::testing::Invoke(::testing::Return(true)));
  EXPECT_CALL(sc, p_read_line)
      .WillOnce(::testing::Invoke(
          ::testing::Return<boost::optional<uint64_t>>({cs_packet})))
      .WillOnce(::testing::Invoke(
          ::testing::Return<boost::optional<uint64_t>>({gyro_packet_1})))
      .WillOnce(::testing::Invoke(
          ::testing::Return<boost::optional<uint64_t>>({gyro_packet_2})))
      .WillRepeatedly(
          ::testing::Invoke(::testing::Return<boost::optional<uint64_t>>({})));

  sc.init();
  sc.start();

  auto cb = [=](uint8_t rstate,
                uint8_t rmotor,
                int8_t raileron,
                int8_t relevator,
                int8_t rrudder) -> void {
    RCPLANE_ENTER();

    ASSERT_EQ(estate, rstate);
    ASSERT_EQ(emotor, rmotor);
    ASSERT_EQ(eaileron, raileron);
    ASSERT_EQ(eelevator, relevator);
    ASSERT_EQ(erudder, rrudder);
  };

  auto gyro_cb = [=](float rgyro_x, float rgyro_y, float rgyro_z) -> void {
    RCPLANE_ENTER();

    ASSERT_EQ(egyro_x, rgyro_x);
    ASSERT_EQ(egyro_y, rgyro_y);
    ASSERT_EQ(egyro_z, rgyro_z);
  };

  sc.register_cs_cb(cb);
  sc.register_gyro_cb(gyro_cb);

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  sc.terminate();
}

TEST(serial_controller_test, stress_test) {
  const uint8_t estate = 0x1F;
  const uint8_t emotor = 0x4F;
  const int8_t eaileron = 0x7F;
  const int8_t eelevator = 0x7F;
  const int8_t erudder = 0x7F;

  const uint32_t gyro_x = 0b01000001001000011001100110011010;
  const uint32_t gyro_y = 0b11000001001000011001100110011010;
  const uint32_t gyro_z = 0b01000001001000011001100110011010;

  const float egyro_x = 10.1f;
  const float egyro_y = -10.1f;
  const float egyro_z = 10.1f;

  const uint64_t cs_packet = estate | emotor << 8 | eaileron << 16
      | eelevator << 24 | (uint64_t)erudder << 32;
  const uint64_t gyro_packet_1 = (uint64_t)gyro_x << 32 | gyro_y;
  const uint64_t gyro_packet_2 = (uint64_t)gyro_z << 32;

  serial_controller_mock sc;
  EXPECT_CALL(sc, p_open_port)
      .WillOnce(::testing::Invoke(::testing::Return(true)));
  EXPECT_CALL(sc, p_handshake_mcu)
      .WillOnce(::testing::Invoke(::testing::Return(true)));
  EXPECT_CALL(sc, p_read_line)
      .WillRepeatedly(
          ::testing::Invoke([cs_packet,
                             gyro_packet_1,
                             gyro_packet_2]() -> boost::optional<uint64_t> {
            static int counter = 0;

            boost::optional<uint64_t> ret{};
            if (counter % 4 == 0) {
              ret = cs_packet;
            } else if (counter % 4 == 1) {
              ret = gyro_packet_1;
            } else if (counter % 4 == 2) {
              ret = gyro_packet_2;
            } else if (counter % 4 == 3) {
              ret = 0x0;
            }
            counter++;
            return ret;
          }));

  sc.init();
  sc.start();

  auto cb = [=](uint8_t rstate,
                uint8_t rmotor,
                int8_t raileron,
                int8_t relevator,
                int8_t rrudder) -> void {
    RCPLANE_ENTER();

    ASSERT_EQ(estate, rstate);
    ASSERT_EQ(emotor, rmotor);
    ASSERT_EQ(eaileron, raileron);
    ASSERT_EQ(eelevator, relevator);
    ASSERT_EQ(erudder, rrudder);
  };

  auto gyro_cb = [=](float rgyro_x, float rgyro_y, float rgyro_z) -> void {
    RCPLANE_ENTER();

    ASSERT_EQ(egyro_x, rgyro_x);
    ASSERT_EQ(egyro_y, rgyro_y);
    ASSERT_EQ(egyro_z, rgyro_z);
  };

  sc.register_cs_cb(cb);
  sc.register_gyro_cb(gyro_cb);

  std::this_thread::sleep_for(std::chrono::seconds(2));
  sc.terminate();
}
