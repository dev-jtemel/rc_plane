#ifndef RCPLANE_AUTOPILOT_AUTOPILOT_MANAGER_HPP
#define RCPLANE_AUTOPILOT_AUTOPILOT_MANAGER_HPP

#include <boost/noncopyable.hpp>

#include "rcplane/autopilot/IAutopilot.hpp"
#include "rcplane/autopilot/ManualAutopilot.hpp"
#include "rcplane/common/Packet.hpp"
#include "rcplane/io/ConfigManager.hpp"

namespace rcplane {
namespace autopilot {

class AutopilotManager : public ::boost::noncopyable {
public:
  AutopilotManager();
  ~AutopilotManager();

  common::ControlSurfacePacket trigger(const common::RcRxPacket &rcRxPacket);

private:
  ManualAutopilot m_manualAutopilot;
  IAutopilot *m_autopilot;
};

}  // namespace autopilot
}  // namespace rcplane

#endif  //RCPLANE_AUTOPILOT_AUTOPILOT_MANAGER_HPP