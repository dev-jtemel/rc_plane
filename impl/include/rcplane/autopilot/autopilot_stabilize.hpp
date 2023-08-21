#ifndef __RCPLANE__AUTOPILOT__AUTOPILOT_STABILIZE_HPP__
#define __RCPLANE__AUTOPILOT__AUTOPILOT_STABILIZE_HPP__

#include "rcplane/autopilot/autopilot_interface.hpp"

namespace rcplane {
namespace autopilot {

class autopilot_stabilize : public interface::autopilot_interface {
public:
  explicit autopilot_stabilize();
  ~autopilot_stabilize();

  void on_entry() override;
  void on_exit() override;

  void handler_impl(common::control_surface_packet *cs_packet,
                    common::imu_packet *imu_packet) override;
};

}  // namespace autopilot
}  // namespace rcplane

#endif  //__RCPLANE__AUTOPILOT__AUTOPILOT_STABILIZE_HPP__