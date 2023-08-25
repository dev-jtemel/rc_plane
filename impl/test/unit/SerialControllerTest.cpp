#include "rcplane/io/SerialController.hpp"

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <gtest/gtest.h>
#include <memory>

#include <random>

#include "rcplane/io/ConfigManager.hpp"

namespace rcplane {
namespace test {

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

TEST_F(SerialControllerFixture, open_Fail) {
  const std::string kReadConfigFile = "configs/config.json";
  m_configManagerRead.loadConfig(kReadConfigFile);

  m_serialControllerRead =
      std::make_unique<io::SerialController>(m_configManagerRead, m_ioService);
  ASSERT_FALSE(m_serialControllerRead->open());
}

TEST_F(SerialControllerFixture, readPacket_FailNotOpen) {
  ASSERT_TRUE(m_serialControllerWrite->open());
  common::HandshakePacket expectedPacket;
  expectedPacket.handshake = getRandomValue<uint8_t>();

  ASSERT_TRUE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
      expectedPacket));

  common::HandshakePacket actualPacket;
  actualPacket = m_serialControllerRead->readPacket<common::HandshakePacket>();
  ASSERT_NE(expectedPacket, actualPacket);

  ASSERT_TRUE(m_serialControllerRead->open());
  actualPacket = m_serialControllerRead->readPacket<common::HandshakePacket>();
  ASSERT_EQ(expectedPacket, actualPacket);
}

TEST_F(SerialControllerFixture, writePacket_FailNotOpen) {
  common::HandshakePacket expectedPacket;
  expectedPacket.handshake = getRandomValue<uint8_t>();

  ASSERT_FALSE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
      expectedPacket));
}

TEST_F(SerialControllerFixture, writePacket_readPacket_handshake) {
  ASSERT_TRUE(m_serialControllerRead->open());
  ASSERT_TRUE(m_serialControllerWrite->open());

  constexpr int kIterations = 100;
  for (int i = 0; i < kIterations; ++i) {
    common::HandshakePacket expectedPacket;
    expectedPacket.handshake = getRandomValue<uint8_t>();

    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
        expectedPacket));

    auto actualPacket =
        m_serialControllerRead->readPacket<common::HandshakePacket>();
    ASSERT_EQ(expectedPacket, actualPacket);
  }
}

TEST_F(SerialControllerFixture, writePacket_readPacket_state) {
  ASSERT_TRUE(m_serialControllerRead->open());
  ASSERT_TRUE(m_serialControllerWrite->open());

  constexpr int kIterations = 100;
  for (int i = 0; i < kIterations; ++i) {
    common::StatePacket expectedPacket;
    expectedPacket.timestamp = getRandomValue<uint32_t>();
    expectedPacket.state = getRandomValue<uint8_t>();

    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::StatePacket>(
        expectedPacket));

    auto actualPacket =
        m_serialControllerRead->readPacket<common::StatePacket>();
    ASSERT_EQ(expectedPacket, actualPacket);
  }
}

TEST_F(SerialControllerFixture, writePacket_readPacket_controlSurfacePacket) {
  ASSERT_TRUE(m_serialControllerRead->open());
  ASSERT_TRUE(m_serialControllerWrite->open());

  constexpr int kIterations = 100;
  for (int i = 0; i < kIterations; ++i) {
    common::ControlSurfacePacket expectedPacket;
    expectedPacket.motorSpeed = getRandomValue<uint8_t>();
    expectedPacket.aileronDeflection = getRandomValue<int8_t>();
    expectedPacket.elevatorDeflection = getRandomValue<int8_t>();
    expectedPacket.rudderDeflection = getRandomValue<int8_t>();

    ASSERT_TRUE(
        m_serialControllerWrite->writePacket<common::ControlSurfacePacket>(
            expectedPacket));

    auto actualPacket =
        m_serialControllerRead->readPacket<common::ControlSurfacePacket>();
    ASSERT_EQ(expectedPacket, actualPacket);
  }
}

TEST_F(SerialControllerFixture, writePacket_readPacket_imuPacket) {
  ASSERT_TRUE(m_serialControllerRead->open());
  ASSERT_TRUE(m_serialControllerWrite->open());

  constexpr int kIterations = 100;
  for (int i = 0; i < kIterations; ++i) {
    common::ImuPacket expectedPacket;
    expectedPacket.accX = getRandomValue<float>();
    expectedPacket.accY = getRandomValue<float>();
    expectedPacket.accZ = getRandomValue<float>();
    expectedPacket.gyroX = getRandomValue<float>();
    expectedPacket.gyroY = getRandomValue<float>();
    expectedPacket.gyroZ = getRandomValue<float>();
    expectedPacket.temperature = getRandomValue<float>();

    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::ImuPacket>(
        expectedPacket));

    auto actualPacket = m_serialControllerRead->readPacket<common::ImuPacket>();
    ASSERT_EQ(expectedPacket, actualPacket);
  }
}

}  // namespace test
}  // namespace rcplane