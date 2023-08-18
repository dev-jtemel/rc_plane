#ifndef __RCPLANE__HW__CONTROL_SURFACE_MANAGER_HPP__
#define __RCPLANE__HW__CONTROL_SURFACE_MANAGER_HPP__

#include <boost/noncopyable.hpp>

#include "rcplane/base_controller.hpp"
#include "rcplane/common/packet.hpp"
#include "rcplane/io/journal.hpp"
#include "rcplane/hw/elevator_controller.hpp"

namespace rcplane {
namespace hw {

class control_surface_manager : public interface::base_controller,
                                public ::boost::noncopyable {
public:
  explicit control_surface_manager()
    : interface::base_controller("cs-manager") {
    RCPLANE_ENTER();
  }
  ~control_surface_manager() { RCPLANE_ENTER(); }

  bool init() override {
    RCPLANE_ENTER();
    return true;
  }

  void start() override { RCPLANE_ENTER(); }

  void terminate() override { RCPLANE_ENTER(); }

  void on(common::control_surface_packet *_cs_packet) {
    RCPLANE_ENTER();
    
    _aileron_controller.on(_cs_packet->aileron);
    _elevator_controller.on(_cs_packet->elevator);
    _rudder_controller.on(_cs_packet->rudder);
  }

  elevator_controller _aileron_controller;
  elevator_controller _elevator_controller;
  elevator_controller _rudder_controller;
};

}  // namespace hw
}  // namespace rcplane

#endif  //__RCPLANE__HW__CONTROL_SURFACE_MANAGER_HPP__