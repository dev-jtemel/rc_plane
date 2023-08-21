#ifndef __RCPLANE__AUTOPILOT__AUTOPILOT_MANAGER_HPP__
#define __RCPLANE__AUTOPILOT__AUTOPILOT_MANAGER_HPP__

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

#include "rcplane/autopilot/autopilot_interface.hpp"
#include "rcplane/autopilot/autopilot_manual.hpp"
#include "rcplane/autopilot/autopilot_stabilize.hpp"
#include "rcplane/base_controller.hpp"
#include "rcplane/common/packet.hpp"

namespace rcplane {
namespace autopilot {

class autopilot_manager : public rcplane::interface::base_controller,
                          public ::boost::noncopyable {
public:
  explicit autopilot_manager(boost::asio::io_context &io);
  ~autopilot_manager();

  bool init() override;

  void start() override;

  void terminate() override;

  interface::autopilot_interface *autopilot();

  void on_state_signal(common::state_packet *state_packet);

  void on_mcu_signal(common::control_surface_packet *cs_packet,
                     common::imu_packet *imu_packet);

protected:
  boost::asio::io_context &_io;

private:
  uint8_t _prev_state{};
  interface::autopilot_interface *_autopilot{nullptr};
  autopilot_manual _ap_manual;
  autopilot_stabilize _ap_stabilize;
};

}  // namespace autopilot
}  // namespace rcplane

#endif  //__RCPLANE__AUTOPILOT__AUTOPILOT_MANAGER_HPP__