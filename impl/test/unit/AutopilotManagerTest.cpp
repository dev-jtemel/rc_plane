#include "rcplane/autopilot/AutopilotManager.hpp"

#include <gtest/gtest.h>
#include <memory>

#include "rcplane/autopilot/AutopilotUtility.hpp"
#include "rcplane/common/Packet.hpp"
#include "rcplane/common/State.hpp"
#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/Journal.hpp"

namespace rcplane {
namespace test {

class AutopilotManagerFixture : public ::testing::Test {
protected:
  void SetUp() override {
    const std::string kConfigFile = "configs/test/unit/generalConfig.json";

    ASSERT_TRUE(m_configManager.loadConfig(kConfigFile));

    m_AutopilotUtility =
        std::make_unique<autopilot::AutopilotUtility>(m_configManager);
    m_AutopilotManager = std::make_unique<autopilot::AutopilotManager>(
        *m_AutopilotUtility.get());
  }

  void TearDown() override {
    m_AutopilotUtility.reset();
    m_AutopilotManager.reset();
  }

  io::ConfigManager m_configManager;
  std::unique_ptr<autopilot::AutopilotUtility> m_AutopilotUtility;
  std::unique_ptr<autopilot::AutopilotManager> m_AutopilotManager;
};

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