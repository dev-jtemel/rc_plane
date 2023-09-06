#include "rcplane/io/telemetry/TelemetryReceiverMQ.hpp"
#include "rcplane/io/telemetry/TelemetryTransmitterMQ.hpp"

#include <gtest/gtest.h>
#include <memory>

#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/Journal.hpp"
#include "rcplane/io/telemetry/TelemetryMessage.hpp"

namespace rcplane {
namespace test {

class TelemetryMQFixture : public ::testing::Test {
protected:
  void SetUp() override {
    const std::string kConfigFile = "configs/test/unit/generalConfig.json";

    ASSERT_TRUE(m_configManager.loadConfig(kConfigFile));

    m_telemetryTransmitter =
        std::make_unique<io::telemetry::TelemetryTransmitterMQ>(
            m_configManager);
    m_telemetryReceiver =
        std::make_unique<io::telemetry::TelemetryReceiverMQ>(m_configManager);
  }

  void TearDown() override {
    m_telemetryTransmitter.reset();
    m_telemetryReceiver.reset();
  }

  io::ConfigManager m_configManager;
  std::unique_ptr<io::telemetry::TelemetryTransmitterMQ> m_telemetryTransmitter;
  std::unique_ptr<io::telemetry::TelemetryReceiverMQ> m_telemetryReceiver;
};

TEST_F(TelemetryMQFixture, init) {
  ASSERT_FALSE(m_telemetryReceiver->init());

  ASSERT_TRUE(m_telemetryTransmitter->init());
  ASSERT_TRUE(m_telemetryReceiver->init());
}

TEST_F(TelemetryMQFixture, transmitter_queueFull) {
  ASSERT_TRUE(m_telemetryTransmitter->init());

  const uint32_t kQueueSize =
      m_configManager.getValue<uint32_t>("rcplane.io.telemetry.mq_size");

  const io::telemetry::message::DebugMessage kDebugMessage;
  const io::telemetry::message::AttitudeMessage kAttitudeMessage;

  for (uint32_t i = 0; i < kQueueSize; ++i) {
    ASSERT_TRUE(m_telemetryTransmitter->sendDebugMessage(kDebugMessage));
    ASSERT_TRUE(m_telemetryTransmitter->sendAttitudeMessage(kAttitudeMessage));
  }

  ASSERT_FALSE(m_telemetryTransmitter->sendDebugMessage(kDebugMessage));
  ASSERT_FALSE(m_telemetryTransmitter->sendAttitudeMessage(kAttitudeMessage));
}

TEST_F(TelemetryMQFixture, receiver_queueFull) {
  ASSERT_TRUE(m_telemetryTransmitter->init());
  ASSERT_TRUE(m_telemetryReceiver->init());

  const uint32_t kQueueSize =
      m_configManager.getValue<uint32_t>("rcplane.io.telemetry.mq_size");

  const io::telemetry::message::DebugMessage kDebugMessage;
  const io::telemetry::message::AttitudeMessage kAttitudeMessage;

  for (uint32_t i = 0; i < kQueueSize; ++i) {
    ASSERT_TRUE(m_telemetryTransmitter->sendDebugMessage(kDebugMessage));
    ASSERT_TRUE(m_telemetryTransmitter->sendAttitudeMessage(kAttitudeMessage));
  }

  for (uint32_t i = 0; i < kQueueSize; ++i) {
    ASSERT_TRUE(m_telemetryReceiver->receiveAttitudeMessage());
    ASSERT_TRUE(m_telemetryReceiver->receiveDebugMessage());
  }
}

}  // namespace test
}  // namespace rcplane