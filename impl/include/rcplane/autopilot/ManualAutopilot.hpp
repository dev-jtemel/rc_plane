#ifndef RCPLANE_AUTOPILOT_MANUAL_AUTOPILOT_HPP
#define RCPLANE_AUTOPILOT_MANUAL_AUTOPILOT_HPP

#include <boost/noncopyable.hpp>

#include "rcplane/autopilot/IAutopilot.hpp"

namespace rcplane {
namespace autopilot {

/**
 * @brief Dummy autopilot that does nothing. Used for full user control.
 */
class ManualAutopilot : public IAutopilot {
public:
  ManualAutopilot();
  virtual ~ManualAutopilot();

  /**
   * @brief nop. Bind the values and return.
   * @param controlSurfacePacket  The control surface packet to modify.
   * @param rcRxPacket The rc rx packet of stick input values.
   */
  void trigger(common::ControlSurfacePacket &controlSurfacePacket,
               const common::RcRxPacket &rcRxPacket);
};

}  // namespace autopilot
}  // namespace rcplane

#endif  // RCPLANE_AUTOPILOT_MANUAL_AUTOPILOT_HPP