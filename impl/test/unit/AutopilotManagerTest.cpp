#include "rcplane/autopilot/AutopilotManager.hpp"

#include <gtest/gtest.h>
#include <memory>

#include "rcplane/common/Packet.hpp"
#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace test {

class AutopilotManagerFixture : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override { m_AutopilotManager.reset(); }

  std::unique_ptr<autopilot::AutopilotManager> m_AutopilotManager =
      std::make_unique<autopilot::AutopilotManager>();
};

TEST_F(AutopilotManagerFixture, zeroToZero) {
  const common::RcRxPacket kRcRxPacket;
  const common::ControlSurfacePacket controlSurfacePacket =
      m_AutopilotManager->trigger(kRcRxPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(0, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(0, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(0, controlSurfacePacket.rudderDeflection);
}

TEST_F(AutopilotManagerFixture, aileronBind) {
  common::RcRxPacket kRcRxPacket;
  kRcRxPacket.aileronStickPosition = 100;
  const common::ControlSurfacePacket controlSurfacePacket =
      m_AutopilotManager->trigger(kRcRxPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(30, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(0, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(0, controlSurfacePacket.rudderDeflection);
}

TEST_F(AutopilotManagerFixture, elevatorBind) {
  common::RcRxPacket kRcRxPacket;
  kRcRxPacket.elevatorStickPosition = -100;
  const common::ControlSurfacePacket controlSurfacePacket =
      m_AutopilotManager->trigger(kRcRxPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(0, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(-40, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(0, controlSurfacePacket.rudderDeflection);
}

TEST_F(AutopilotManagerFixture, rudderBind) {
  common::RcRxPacket kRcRxPacket;
  kRcRxPacket.rudderStickPosition = 50;
  m_AutopilotManager->trigger(kRcRxPacket);
  const common::ControlSurfacePacket controlSurfacePacket =
      m_AutopilotManager->trigger(kRcRxPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(0, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(0, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(10, controlSurfacePacket.rudderDeflection);
}

TEST_F(AutopilotManagerFixture, allBind) {
  common::RcRxPacket kRcRxPacket;
  kRcRxPacket.motorStickPosition = 100;
  kRcRxPacket.aileronStickPosition = -10;
  kRcRxPacket.elevatorStickPosition = 50;
  kRcRxPacket.rudderStickPosition = -25;
  m_AutopilotManager->trigger(kRcRxPacket);
  const common::ControlSurfacePacket controlSurfacePacket =
      m_AutopilotManager->trigger(kRcRxPacket);

  EXPECT_EQ(100, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(-3, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(20, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(-5, controlSurfacePacket.rudderDeflection);
}

}  // namespace test
}  // namespace rcplane