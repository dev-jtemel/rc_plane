#include "rcplane/io/SerialController.hpp"

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <gtest/gtest.h>
#include <memory>

#include "PacketTestUtils.hpp"
#include "rcplane/common/Packet.hpp"
#include "rcplane/io/Journal.hpp"
#include "rcplane/io/ConfigManager.hpp"

namespace rcplane {
namespace test {

class SerialControllerFixture : public ::testing::Test {
protected:
  void SetUp() override {
    const std::string kReadConfigFile =
        "configs/test/unit/serialControllerRead.json";
    const std::string kWriteConfigFile =
        "configs/test/unit/serialControllerWrite.json";

    m_configManagerRead.loadConfig(kReadConfigFile);
    m_configManagerWrite.loadConfig(kWriteConfigFile);

    m_serialControllerRead =
        std::make_unique<io::SerialController>(m_configManagerRead,
                                               m_ioService);
    m_serialControllerWrite =
        std::make_unique<io::SerialController>(m_configManagerWrite,
                                               m_ioService);

    m_ioThread = boost::thread([&]() { m_ioService.run(); });
  }
  void TearDown() override {
    m_worker.reset();
    m_worker.~executor_work_guard();
    m_worker.~executor_work_guard();

    m_ioService.stop();
    m_ioThread.join();

    m_serialControllerRead.reset();
    m_serialControllerWrite.reset();
  }

  io::ConfigManager m_configManagerRead;
  io::ConfigManager m_configManagerWrite;
  boost::thread m_ioThread;
  boost::asio::io_service m_ioService{};
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
      m_worker = boost::asio::make_work_guard(m_ioService);
  std::unique_ptr<io::SerialController> m_serialControllerRead{};
  std::unique_ptr<io::SerialController> m_serialControllerWrite{};
};

TEST_F(SerialControllerFixture, open) {
  ASSERT_TRUE(m_serialControllerRead->open());
  ASSERT_TRUE(m_serialControllerWrite->open());
}

TEST_F(SerialControllerFixture, open_fail) {
  const std::string kReadConfigFile = "configs/config.json";
  m_configManagerRead.loadConfig(kReadConfigFile);

  m_serialControllerRead =
      std::make_unique<io::SerialController>(m_configManagerRead, m_ioService);
  ASSERT_FALSE(m_serialControllerRead->open());
}

TEST_F(SerialControllerFixture, open_flush) {
  ASSERT_TRUE(m_serialControllerWrite->open());

  // We need to write the string value specified in the config. So, we write char
  // by char in a HandshakePacket since it stores exactly one char (i.e. uint8_t).
  std::string cFlushTerminationStr = m_configManagerWrite.getValue<std::string>(
      "rcplane.common.handshake_termination_string");
  for (auto &c : cFlushTerminationStr) {
    common::HandshakePacket charPacket;
    charPacket.handshake = static_cast<uint8_t>(c);
    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
        charPacket));
  }

  ASSERT_TRUE(m_serialControllerRead->open());
  ASSERT_TRUE(m_serialControllerRead->flush());
}

TEST_F(SerialControllerFixture, readPacket_FailNotOpen) {
  ASSERT_TRUE(m_serialControllerWrite->open());
  const common::HandshakePacket kExpectedHandshakePacket =
      util::createPacket<common::HandshakePacket>();
  const common::RcRxPacket kExpectedRcRxPacket =
      util::createPacket<common::RcRxPacket>();
  const common::ControlSurfacePacket kExpectedControlSurfacePacket =
      util::createPacket<common::ControlSurfacePacket>();
  const common::ImuPacket kExpectedImuPacket =
      util::createPacket<common::ImuPacket>();

  ASSERT_TRUE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
      kExpectedHandshakePacket));
  ASSERT_TRUE(m_serialControllerWrite->writePacket<common::RcRxPacket>(
      kExpectedRcRxPacket));
  ASSERT_TRUE(
      m_serialControllerWrite->writePacket<common::ControlSurfacePacket>(
          kExpectedControlSurfacePacket));
  ASSERT_TRUE(m_serialControllerWrite->writePacket<common::ImuPacket>(
      kExpectedImuPacket));

  common::HandshakePacket actualHandshakePacket;
  common::RcRxPacket actualRcRxPacket;
  common::ControlSurfacePacket actualControlSurfacePacket;
  common::ImuPacket actualImuPacket;

  actualHandshakePacket =
      m_serialControllerRead->readPacket<common::HandshakePacket>().packet;
  actualRcRxPacket =
      m_serialControllerRead->readPacket<common::RcRxPacket>().packet;
  actualControlSurfacePacket =
      m_serialControllerRead->readPacket<common::ControlSurfacePacket>().packet;
  actualImuPacket =
      m_serialControllerRead->readPacket<common::ImuPacket>().packet;

  ASSERT_NE(kExpectedHandshakePacket, actualHandshakePacket);
  ASSERT_NE(kExpectedRcRxPacket, actualRcRxPacket);
  ASSERT_NE(kExpectedControlSurfacePacket, actualControlSurfacePacket);
  ASSERT_NE(kExpectedImuPacket, actualImuPacket);

  ASSERT_TRUE(m_serialControllerRead->open());
  actualHandshakePacket =
      m_serialControllerRead->readPacket<common::HandshakePacket>().packet;
  actualRcRxPacket =
      m_serialControllerRead->readPacket<common::RcRxPacket>().packet;
  actualControlSurfacePacket =
      m_serialControllerRead->readPacket<common::ControlSurfacePacket>().packet;
  actualImuPacket =
      m_serialControllerRead->readPacket<common::ImuPacket>().packet;

  RCPLANE_LOG_TEST(debug, actualHandshakePacket);
  RCPLANE_LOG_TEST(debug, actualRcRxPacket);
  RCPLANE_LOG_TEST(debug, actualControlSurfacePacket);
  RCPLANE_LOG_TEST(debug, actualImuPacket);

  ASSERT_EQ(kExpectedHandshakePacket, actualHandshakePacket);
  ASSERT_EQ(kExpectedRcRxPacket, actualRcRxPacket);
  ASSERT_EQ(kExpectedControlSurfacePacket, actualControlSurfacePacket);
  ASSERT_EQ(kExpectedImuPacket, actualImuPacket);
}

TEST_F(SerialControllerFixture, writePacket_FailNotOpen) {
  const common::HandshakePacket kExpectedPacket =
      util::createPacket<common::HandshakePacket>();

  ASSERT_FALSE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
      kExpectedPacket));
}

TEST_F(SerialControllerFixture, writePacket_readPacket_handshake) {
  ASSERT_TRUE(m_serialControllerRead->open());
  ASSERT_TRUE(m_serialControllerWrite->open());

  constexpr int kIterations = 100;
  for (int i = 0; i < kIterations; ++i) {
    const common::HandshakePacket kExpectedPacket =
        util::createPacket<common::HandshakePacket>();

    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
        kExpectedPacket));

    const auto kActualPacket =
        m_serialControllerRead->readPacket<common::HandshakePacket>().packet;
    ASSERT_EQ(kExpectedPacket, kActualPacket);
  }
}

TEST_F(SerialControllerFixture, writePacket_readPacket_RcRx) {
  ASSERT_TRUE(m_serialControllerRead->open());
  ASSERT_TRUE(m_serialControllerWrite->open());

  constexpr int kIterations = 100;
  for (int i = 0; i < kIterations; ++i) {
    const common::RcRxPacket kExpectedPacket =
        util::createPacket<common::RcRxPacket>();

    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::RcRxPacket>(
        kExpectedPacket));

    const auto kActualPacket =
        m_serialControllerRead->readPacket<common::RcRxPacket>().packet;
    ASSERT_EQ(kExpectedPacket, kActualPacket);
  }
}

TEST_F(SerialControllerFixture, writePacket_readPacket_controlSurfacePacket) {
  ASSERT_TRUE(m_serialControllerRead->open());
  ASSERT_TRUE(m_serialControllerWrite->open());

  constexpr int kIterations = 100;
  for (int i = 0; i < kIterations; ++i) {
    const common::ControlSurfacePacket kExpectedPacket =
        util::createPacket<common::ControlSurfacePacket>();
    ASSERT_TRUE(
        m_serialControllerWrite->writePacket<common::ControlSurfacePacket>(
            kExpectedPacket));

    const auto kActualPacket =
        m_serialControllerRead->readPacket<common::ControlSurfacePacket>()
            .packet;
    ASSERT_EQ(kExpectedPacket, kActualPacket);
  }
}

TEST_F(SerialControllerFixture, writePacket_readPacket_imuPacket) {
  ASSERT_TRUE(m_serialControllerRead->open());
  ASSERT_TRUE(m_serialControllerWrite->open());

  constexpr int kIterations = 100;
  for (int i = 0; i < kIterations; ++i) {
    const common::ImuPacket kExpectedPacket =
        util::createPacket<common::ImuPacket>();

    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::ImuPacket>(
        kExpectedPacket));

    const auto kActualPacket =
        m_serialControllerRead->readPacket<common::ImuPacket>().packet;
    ASSERT_EQ(kExpectedPacket, kActualPacket);
  }
}

TEST_F(SerialControllerFixture, writePacket_before_readPacketStress) {
  ASSERT_TRUE(m_serialControllerRead->open());
  ASSERT_TRUE(m_serialControllerWrite->open());

  constexpr int kIterations = 100;
  std::vector<common::ImuPacket> expectedPackets;
  for (int i = 0; i < kIterations; ++i) {
    const common::ImuPacket kExpectedPacket =
        util::createPacket<common::ImuPacket>();

    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::ImuPacket>(
        kExpectedPacket));
    expectedPackets.push_back(kExpectedPacket);
  }

  for (const auto &kExpectedPacket : expectedPackets) {
    const auto kActualPacket =
        m_serialControllerRead->readPacket<common::ImuPacket>().packet;
    ASSERT_EQ(kExpectedPacket, kActualPacket);
  }
}

}  // namespace test
}  // namespace rcplane