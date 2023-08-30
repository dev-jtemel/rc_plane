#include "rcplane/autopilot/ManualAutopilot.hpp"

#include <gtest/gtest.h>
#include <memory>

#include "rcplane/autopilot/AutopilotUtility.hpp"
#include "rcplane/common/Packet.hpp"
#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace test {

class ManualAutopilotFixture : public ::testing::Test {
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

    m_AutopilotUtility =
        std::make_unique<autopilot::AutopilotUtility>(m_configManager);
    m_manualAutopilot =
        std::make_unique<autopilot::ManualAutopilot>(*m_AutopilotUtility.get());
  }

  void TearDown() override {
    m_AutopilotUtility.reset();
    m_manualAutopilot.reset();
  }

  const int8_t kMaxStickPosition = 100;
  const int8_t kMinStickPosition = -100;

  uint8_t c_maxThrottle{};
  int8_t c_maxAileronDeflection{};
  int8_t c_maxElevatorDeflection{};
  int8_t c_maxRudderDeflection{};

  io::ConfigManager m_configManager;
  std::unique_ptr<autopilot::AutopilotUtility> m_AutopilotUtility;
  std::unique_ptr<autopilot::ManualAutopilot> m_manualAutopilot;
};

TEST_F(ManualAutopilotFixture, zeroToZero) {
  common::ControlSurfacePacket controlSurfacePacket;
  const common::ImuPacket kImuPacket;
  const common::RcRxPacket kRcRxPacket;

  m_manualAutopilot->trigger(controlSurfacePacket, kRcRxPacket, kImuPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(0, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(0, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(0, controlSurfacePacket.rudderDeflection);
}

TEST_F(ManualAutopilotFixture, aileronBind) {
  common::ControlSurfacePacket controlSurfacePacket;
  const common::ImuPacket kImuPacket;
  common::RcRxPacket rcRxPacket;
  rcRxPacket.aileronStickPosition = kMaxStickPosition;

  m_manualAutopilot->trigger(controlSurfacePacket, rcRxPacket, kImuPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(c_maxAileronDeflection, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(0, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(0, controlSurfacePacket.rudderDeflection);
}

TEST_F(ManualAutopilotFixture, elevatorBind) {
  common::ControlSurfacePacket controlSurfacePacket;
  const common::ImuPacket kImuPacket;
  common::RcRxPacket rcRxPacket;
  rcRxPacket.elevatorStickPosition = kMinStickPosition;

  m_manualAutopilot->trigger(controlSurfacePacket, rcRxPacket, kImuPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(0, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(-c_maxElevatorDeflection, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(0, controlSurfacePacket.rudderDeflection);
}

TEST_F(ManualAutopilotFixture, rudderBind) {
  common::ControlSurfacePacket controlSurfacePacket;
  const common::ImuPacket kImuPacket;
  common::RcRxPacket rcRxPacket;
  rcRxPacket.rudderStickPosition = kMaxStickPosition / 2;

  m_manualAutopilot->trigger(controlSurfacePacket, rcRxPacket, kImuPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(0, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(0, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(c_maxRudderDeflection / 2, controlSurfacePacket.rudderDeflection);
}

TEST_F(ManualAutopilotFixture, allBind) {
  common::ControlSurfacePacket controlSurfacePacket;
  const common::ImuPacket kImuPacket;
  common::RcRxPacket rcRxPacket;
  rcRxPacket.motorStickPosition = kMaxStickPosition;
  rcRxPacket.aileronStickPosition = kMinStickPosition / 10;
  rcRxPacket.elevatorStickPosition = kMaxStickPosition / 2;
  rcRxPacket.rudderStickPosition = kMinStickPosition / 4;

  m_manualAutopilot->trigger(controlSurfacePacket, rcRxPacket, kImuPacket);

  EXPECT_EQ(c_maxThrottle, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(static_cast<int8_t>(-c_maxElevatorDeflection / 10),
            controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(static_cast<int8_t>(c_maxElevatorDeflection / 2),
            controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(static_cast<int8_t>(-c_maxRudderDeflection / 4),
            controlSurfacePacket.rudderDeflection);
}

}  // namespace test
}  // namespace rcplane