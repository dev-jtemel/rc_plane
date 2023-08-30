#include "rcplane/autopilot/ManualAutopilot.hpp"

#include <gtest/gtest.h>
#include <memory>

#include "rcplane/common/Packet.hpp"
#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace test {

class ManualAutopilotFixture : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override { m_manualAutopilot.reset(); }

  std::unique_ptr<autopilot::ManualAutopilot> m_manualAutopilot =
      std::make_unique<autopilot::ManualAutopilot>();
};

TEST_F(ManualAutopilotFixture, zeroToZero) {
  const common::ImuPacket kImuPacket;
  common::ControlSurfacePacket controlSurfacePacket;
  const common::RcRxPacket kRcRxPacket;

  m_manualAutopilot->trigger(controlSurfacePacket, kRcRxPacket, kImuPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(0, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(0, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(0, controlSurfacePacket.rudderDeflection);
}

TEST_F(ManualAutopilotFixture, aileronBind) {
  const common::ImuPacket kImuPacket;
  common::ControlSurfacePacket controlSurfacePacket;
  common::RcRxPacket kRcRxPacket;
  kRcRxPacket.aileronStickPosition = 100;

  m_manualAutopilot->trigger(controlSurfacePacket, kRcRxPacket, kImuPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(30, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(0, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(0, controlSurfacePacket.rudderDeflection);
}

TEST_F(ManualAutopilotFixture, elevatorBind) {
  const common::ImuPacket kImuPacket;
  common::ControlSurfacePacket controlSurfacePacket;
  common::RcRxPacket kRcRxPacket;
  kRcRxPacket.elevatorStickPosition = -100;

  m_manualAutopilot->trigger(controlSurfacePacket, kRcRxPacket, kImuPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(0, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(-40, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(0, controlSurfacePacket.rudderDeflection);
}

TEST_F(ManualAutopilotFixture, rudderBind) {
  const common::ImuPacket kImuPacket;
  common::ControlSurfacePacket controlSurfacePacket;
  common::RcRxPacket kRcRxPacket;
  kRcRxPacket.rudderStickPosition = 50;

  m_manualAutopilot->trigger(controlSurfacePacket, kRcRxPacket, kImuPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(0, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(0, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(10, controlSurfacePacket.rudderDeflection);
}

TEST_F(ManualAutopilotFixture, allBind) {
  const common::ImuPacket kImuPacket;
  common::ControlSurfacePacket controlSurfacePacket;
  common::RcRxPacket kRcRxPacket;
  kRcRxPacket.motorStickPosition = 100;
  kRcRxPacket.aileronStickPosition = -10;
  kRcRxPacket.elevatorStickPosition = 50;
  kRcRxPacket.rudderStickPosition = -25;

  m_manualAutopilot->trigger(controlSurfacePacket, kRcRxPacket, kImuPacket);

  EXPECT_EQ(100, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(-3, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(20, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(-5, controlSurfacePacket.rudderDeflection);
}

}  // namespace test
}  // namespace rcplane