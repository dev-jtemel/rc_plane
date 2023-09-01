#include "rcplane/som/SomController.hpp"

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/thread.hpp>
#include <future>
#include <gtest/gtest.h>
#include <memory>
#include <thread>

#include "PacketTestUtils.hpp"
#include "rcplane/common/Packet.hpp"
#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/Journal.hpp"
#include "rcplane/io/SerialController.hpp"

namespace rcplane {
namespace test {

class SomControllerFixture : public ::testing::Test {
protected:
  void SetUp() override {
    m_somController = std::make_unique<som::SomController>(kConfigPath);

    const std::string kWriteConfigFile =
        "configs/test/unit/serialControllerWrite.json";
    m_configManagerWrite.loadConfig(kWriteConfigFile);

    m_serialControllerWrite =
        std::make_unique<io::SerialController>(m_configManagerWrite,
                                               m_somController->getIoService());
  }

  void TearDown() override {
    m_serialControllerWrite.reset();
    m_somController.reset();
  }

  const std::string kConfigPath = "configs/test/unit/generalConfig.json";
  io::ConfigManager m_configManagerWrite;
  std::unique_ptr<io::SerialController> m_serialControllerWrite{};
  std::unique_ptr<som::SomController> m_somController{};
};

TEST_F(SomControllerFixture, handshakeMCU_noResponse) {
  ASSERT_TRUE(m_serialControllerWrite->open());
  m_somController->startIoThread();

  const uint8_t kFlushStr[] = {'f', 'l', 'u', 's', 'h', '\r', '\n'};
  for (size_t i = 0; i < sizeof(kFlushStr); i++) {
    common::HandshakePacket handshakePacket{kFlushStr[i]};
    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
        handshakePacket));
  }

  ASSERT_FALSE(m_somController->handshakeMCU());
}

TEST_F(SomControllerFixture, handshakeMCU_responseInvalid) {
  ASSERT_TRUE(m_serialControllerWrite->open());
  m_somController->startIoThread();

  const uint8_t kFlushStr[] = {'f', 'l', 'u', 's', 'h', '\r', '\n'};
  for (size_t i = 0; i < sizeof(kFlushStr); i++) {
    common::HandshakePacket handshakePacket{kFlushStr[i]};
    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
        handshakePacket));
  }

  // Simulate MCU reading and returning the handshake incorrect response
  auto mcuFuture = std::async([this] {
    const auto handshakePacket =
        m_serialControllerWrite->readPacket<common::HandshakePacket>().packet;

    const common::HandshakePacket kHandshakeResponse{31U};
    constexpr uint32_t kResponseCount = 5U;
    for (int i = 0; i < kResponseCount; i++) {
      ASSERT_TRUE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
          kHandshakeResponse));
    }
  });

  ASSERT_FALSE(m_somController->handshakeMCU());

  mcuFuture.wait();
}

TEST_F(SomControllerFixture, handshakeMCU_responseSuccess) {
  ASSERT_TRUE(m_serialControllerWrite->open());
  m_somController->startIoThread();

  const uint8_t kFlushStr[] = {'f', 'l', 'u', 's', 'h', '\r', '\n'};
  for (size_t i = 0; i < sizeof(kFlushStr); i++) {
    common::HandshakePacket handshakePacket{kFlushStr[i]};
    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
        handshakePacket));
  }

  // Simulate MCU reading and returning the handshake response
  auto mcuFuture = std::async([this] {
    const auto handshakePacket =
        m_serialControllerWrite->readPacket<common::HandshakePacket>().packet;

    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
        handshakePacket));
  });

  ASSERT_TRUE(m_somController->handshakeMCU());

  mcuFuture.wait();
}

TEST_F(SomControllerFixture, mainLoop_noResponse) {
  ASSERT_TRUE(m_serialControllerWrite->open());
  m_somController->startIoThread();

  const uint8_t kFlushStr[] = {'f', 'l', 'u', 's', 'h', '\r', '\n'};
  for (size_t i = 0; i < sizeof(kFlushStr); i++) {
    common::HandshakePacket handshakePacket{kFlushStr[i]};
    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
        handshakePacket));
  }

  // Simulate MCU reading and returning the handshake response
  auto mcuHandshakeFuture = std::async([this] {
    const auto handshakePacket =
        m_serialControllerWrite->readPacket<common::HandshakePacket>().packet;
    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
        handshakePacket));
  });

  auto mcuLoopFuture = std::async([this] {
    constexpr uint32_t kLoopDelayMs = 1000U;
    std::this_thread::sleep_for(std::chrono::milliseconds(kLoopDelayMs));
    std::raise(SIGINT);
  });

  ASSERT_TRUE(m_somController->handshakeMCU());

  m_somController->runMainLoop();

  mcuHandshakeFuture.wait();
  mcuLoopFuture.wait();
}

TEST_F(SomControllerFixture, mainLoop_oneIteration) {
  ASSERT_TRUE(m_serialControllerWrite->open());
  m_somController->startIoThread();

  const uint8_t kFlushStr[] = {'f', 'l', 'u', 's', 'h', '\r', '\n'};
  for (size_t i = 0; i < sizeof(kFlushStr); i++) {
    common::HandshakePacket handshakePacket{kFlushStr[i]};
    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
        handshakePacket));
  }

  // Simulate MCU reading and returning the handshake response
  auto mcuHandshakeFuture = std::async([this] {
    const auto handshakePacket =
        m_serialControllerWrite->readPacket<common::HandshakePacket>().packet;
    ASSERT_TRUE(m_serialControllerWrite->writePacket<common::HandshakePacket>(
        handshakePacket));
  });

  auto mcuLoopFuture = std::async([this] {
    constexpr uint32_t kLoopDelayMs = 100U;
    std::this_thread::sleep_for(std::chrono::milliseconds(kLoopDelayMs));

    const common::RcRxPacket kRcRxPacket =
        util::createPacket<common::RcRxPacket>();
    const common::ImuPacket kImuPacket =
        util::createPacket<common::ImuPacket>();

    ASSERT_TRUE(
        m_serialControllerWrite->writePacket<common::RcRxPacket>(kRcRxPacket));
    ASSERT_TRUE(
        m_serialControllerWrite->writePacket<common::ImuPacket>(kImuPacket));

    auto controlSurfaceResponse =
        m_serialControllerWrite->readPacket<common::ControlSurfacePacket>();
    ASSERT_TRUE(!controlSurfaceResponse.didTimeout);

    std::this_thread::sleep_for(std::chrono::milliseconds(kLoopDelayMs));
    std::raise(SIGINT);
  });

  ASSERT_TRUE(m_somController->handshakeMCU());

  m_somController->runMainLoop();

  mcuHandshakeFuture.wait();
  mcuLoopFuture.wait();
}

}  // namespace test
}  // namespace rcplane