#include "rcplane/autopilot/AutopilotManager.hpp"

#include <gtest/gtest.h>
#include <memory>

#include "rcplane/common/Packet.hpp"
#include "rcplane/common/State.hpp"
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
  const common::ImuPacket kImuPacket;
  const common::RcRxPacket kRcRxPacket;

  const common::ControlSurfacePacket controlSurfacePacket =
      m_AutopilotManager->trigger(kRcRxPacket, kImuPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(0, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(0, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(0, controlSurfacePacket.rudderDeflection);
}

TEST_F(AutopilotManagerFixture, aileronBind) {
  const common::ImuPacket kImuPacket;
  common::RcRxPacket rcRxPacket;
  rcRxPacket.aileronStickPosition = 100;

  const common::ControlSurfacePacket controlSurfacePacket =
      m_AutopilotManager->trigger(rcRxPacket, kImuPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(30, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(0, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(0, controlSurfacePacket.rudderDeflection);
}

TEST_F(AutopilotManagerFixture, elevatorBind) {
  const common::ImuPacket kImuPacket;
  common::RcRxPacket rcRxPacket;
  rcRxPacket.elevatorStickPosition = -100;

  const common::ControlSurfacePacket controlSurfacePacket =
      m_AutopilotManager->trigger(rcRxPacket, kImuPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(0, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(-40, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(0, controlSurfacePacket.rudderDeflection);
}

TEST_F(AutopilotManagerFixture, rudderBind) {
  const common::ImuPacket kImuPacket;
  common::RcRxPacket rcRxPacket;
  rcRxPacket.rudderStickPosition = 50;

  const common::ControlSurfacePacket controlSurfacePacket =
      m_AutopilotManager->trigger(rcRxPacket, kImuPacket);

  EXPECT_EQ(0, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(0, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(0, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(10, controlSurfacePacket.rudderDeflection);
}

TEST_F(AutopilotManagerFixture, allBind) {
  const common::ImuPacket kImuPacket;
  common::RcRxPacket rcRxPacket;
  rcRxPacket.motorStickPosition = 100;
  rcRxPacket.aileronStickPosition = -10;
  rcRxPacket.elevatorStickPosition = 50;
  rcRxPacket.rudderStickPosition = -25;

  const common::ControlSurfacePacket controlSurfacePacket =
      m_AutopilotManager->trigger(rcRxPacket, kImuPacket);

  EXPECT_EQ(100, controlSurfacePacket.motorSpeed);
  EXPECT_EQ(-3, controlSurfacePacket.aileronDeflection);
  EXPECT_EQ(20, controlSurfacePacket.elevatorDeflection);
  EXPECT_EQ(-5, controlSurfacePacket.rudderDeflection);
}

TEST_F(AutopilotManagerFixture, handleState_noTransition) {
  const common::ImuPacket kImuPacket;
  const common::RcRxPacket kRcRxPacket;

  ASSERT_TRUE(m_AutopilotManager->isInManualMode());
  (void)m_AutopilotManager->trigger(kRcRxPacket, kImuPacket);
  ASSERT_TRUE(m_AutopilotManager->isInManualMode());
}

TEST_F(AutopilotManagerFixture, handleState_noTransitionUserInput) {
  const common::ImuPacket kImuPacket;
  const common::RcRxPacket kRcRxPacket;
  common::RcRxPacket rcRxPacket;
  rcRxPacket.state = common::state::kUSER_ROLL;

  ASSERT_TRUE(m_AutopilotManager->isInManualMode());
  (void)m_AutopilotManager->trigger(kRcRxPacket, kImuPacket);
  ASSERT_TRUE(m_AutopilotManager->isInManualMode());
  (void)m_AutopilotManager->trigger(rcRxPacket, kImuPacket);
  ASSERT_TRUE(m_AutopilotManager->isInManualMode());
}

TEST_F(AutopilotManagerFixture, handleState_TransitionToStabilize) {
  const common::ImuPacket kImuPacket;
  const common::RcRxPacket kRcRxPacket;
  common::RcRxPacket rcRxPacket;
  rcRxPacket.state = common::state::kASSISTANCE_FLAG;

  ASSERT_TRUE(m_AutopilotManager->isInManualMode());
  (void)m_AutopilotManager->trigger(kRcRxPacket, kImuPacket);
  ASSERT_TRUE(m_AutopilotManager->isInManualMode());
  (void)m_AutopilotManager->trigger(rcRxPacket, kImuPacket);
  ASSERT_TRUE(m_AutopilotManager->isInStabilizeMode());
}

TEST_F(AutopilotManagerFixture, handleState_TransitionToStabilizeBackToManual) {
  const common::ImuPacket kImuPacket;
  const common::RcRxPacket kRcRxPacket;
  common::RcRxPacket rcRxPacket;
  rcRxPacket.state = common::state::kASSISTANCE_FLAG;

  ASSERT_TRUE(m_AutopilotManager->isInManualMode());
  (void)m_AutopilotManager->trigger(kRcRxPacket, kImuPacket);
  ASSERT_TRUE(m_AutopilotManager->isInManualMode());
  (void)m_AutopilotManager->trigger(rcRxPacket, kImuPacket);
  ASSERT_TRUE(m_AutopilotManager->isInStabilizeMode());
  (void)m_AutopilotManager->trigger(kRcRxPacket, kImuPacket);
  ASSERT_TRUE(m_AutopilotManager->isInManualMode());
}

}  // namespace test
}  // namespace rcplane