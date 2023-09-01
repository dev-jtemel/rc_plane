#include "rcplane/autopilot/StabilizeAutopilot.hpp"

#include <gtest/gtest.h>
#include <memory>

#include "rcplane/autopilot/AutopilotUtility.hpp"
#include "rcplane/common/Packet.hpp"
#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace test {

class StabilizeAutopilotFixture : public ::testing::Test {
protected:
  void SetUp() override {
    const std::string kConfigFile = "configs/test/unit/generalConfig.json";

    ASSERT_TRUE(m_configManager.loadConfig(kConfigFile));

    c_maxThrottle =
        m_configManager.getValue<uint8_t>("rcplane.autopilot.max_throttle");
    c_maxAileronDeflection = m_configManager.getValue<int8_t>(
        "rcplane.autopilot.max_aileron_deflection");
    c_maxElevatorDeflection = m_configManager.getValue<int8_t>(
        "rcplane.autopilot.max_elevator_deflection");
    c_maxRudderDeflection = m_configManager.getValue<int8_t>(
        "rcplane.autopilot.max_rudder_deflection");

    m_autopilotUtility =
        std::make_unique<autopilot::AutopilotUtility>(m_configManager);
    m_stabilizeAutopilot = std::make_unique<autopilot::StabilizeAutopilot>(
        *m_autopilotUtility.get());
  }

  void TearDown() override {
    m_autopilotUtility.reset();
    m_stabilizeAutopilot.reset();
  }

  const int8_t kMaxStickPosition = 100;
  const int8_t kMinStickPosition = -100;

  uint8_t c_maxThrottle{};
  int8_t c_maxAileronDeflection{};
  int8_t c_maxElevatorDeflection{};
  int8_t c_maxRudderDeflection{};

  io::ConfigManager m_configManager;
  std::unique_ptr<autopilot::AutopilotUtility> m_autopilotUtility;
  std::unique_ptr<autopilot::StabilizeAutopilot> m_stabilizeAutopilot;
};

TEST_F(StabilizeAutopilotFixture, zeroToZero) {
  common::ControlSurfacePacket controlSurfacePacket;
  const common::ImuPacket kImuPacket;
  const common::RcRxPacket kRcRxPacket;

  m_stabilizeAutopilot->trigger(controlSurfacePacket, kRcRxPacket, kImuPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(0, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(0, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(0, controlSurfacePacket.rudderDeflection);
}

TEST_F(StabilizeAutopilotFixture, aileronBind) {
  common::ControlSurfacePacket controlSurfacePacket;
  const common::RcRxPacket kRcRxPacket;
  common::ImuPacket imuPacket;
  imuPacket.gyroX = -35;

  constexpr uint32_t kLoopCount = 1000U;
  for (auto i = 0U; i < kLoopCount; ++i) {
    m_stabilizeAutopilot->trigger(controlSurfacePacket, kRcRxPacket, imuPacket);
  }

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_GE(c_maxAileronDeflection, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(0, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(0, controlSurfacePacket.rudderDeflection);
}

TEST_F(StabilizeAutopilotFixture, elevatorBind) {
  common::ControlSurfacePacket controlSurfacePacket;
  const common::RcRxPacket kRcRxPacket;
  common::ImuPacket imuPacket;
  imuPacket.gyroY = 35;

  constexpr uint32_t kLoopCount = 1000U;
  for (auto i = 0U; i < kLoopCount; ++i) {
    m_stabilizeAutopilot->trigger(controlSurfacePacket, kRcRxPacket, imuPacket);
  }

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(0, controlSurfacePacket.aileronDeflection);
  EXPECT_LE(-c_maxElevatorDeflection, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(0, controlSurfacePacket.rudderDeflection);
}

TEST_F(StabilizeAutopilotFixture, rudderBind) {
  common::ControlSurfacePacket controlSurfacePacket;
  const common::ImuPacket kImuPacket;
  common::RcRxPacket rcRxPacket;
  rcRxPacket.rudderStickPosition = kMaxStickPosition / 2;

  m_stabilizeAutopilot->trigger(controlSurfacePacket, rcRxPacket, kImuPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(0, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(0, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(c_maxRudderDeflection / 2, controlSurfacePacket.rudderDeflection);
}

TEST_F(StabilizeAutopilotFixture, allBind) {
  common::ControlSurfacePacket controlSurfacePacket;
  common::ImuPacket imuPacket;
  common::RcRxPacket rcRxPacket;
  rcRxPacket.motorStickPosition = kMaxStickPosition;
  rcRxPacket.rudderStickPosition = kMinStickPosition / 4;

  imuPacket.gyroX = 10;
  imuPacket.gyroY = -5;

  constexpr uint32_t kLoopCount = 1000U;
  for (auto i = 0U; i < kLoopCount; ++i) {
    m_stabilizeAutopilot->trigger(controlSurfacePacket, rcRxPacket, imuPacket);
  }

  EXPECT_EQ(c_maxThrottle, controlSurfacePacket.motorSpeed);
  EXPECT_GE(static_cast<int8_t>(-c_maxElevatorDeflection / 10),
            controlSurfacePacket.aileronDeflection);
  EXPECT_GE(static_cast<int8_t>(c_maxElevatorDeflection / 2),
            controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(static_cast<int8_t>(-c_maxRudderDeflection / 4),
            controlSurfacePacket.rudderDeflection);
}

}  // namespace test
}  // namespace rcplane