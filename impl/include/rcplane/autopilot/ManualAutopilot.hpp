#ifndef RCPLANE_AUTOPILOT_MANUAL_AUTOPILOT_HPP
#define RCPLANE_AUTOPILOT_MANUAL_AUTOPILOT_HPP

#include <boost/noncopyable.hpp>

#include "rcplane/autopilot/IAutopilot.hpp"

namespace rcplane {
namespace autopilot {

class ManualAutopilot : public IAutopilot {
public:
  ManualAutopilot();

  virtual ~ManualAutopilot();

  void trigger(common::ControlSurfacePacket &controlSurfacePacket,
               const common::RcRxPacket &rcRxPacket);
};

}  // namespace autopilot
}  // namespace rcplane

#endif  // RCPLANE_AUTOPILOT_MANUAL_AUTOPILOT_HPP