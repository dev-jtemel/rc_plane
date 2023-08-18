#ifndef __RCPLANE__HW__ELEVATOR_CONTROLLER_HPP__
#define __RCPLANE__HW__ELEVATOR_CONTROLLER_HPP__

#include <boost/noncopyable.hpp>

#include "rcplane/base_controller.hpp"
#include "rcplane/io/config_manager.hpp"
#include "rcplane/common/packet.hpp"
#include "rcplane/io/journal.hpp"

namespace rcplane {
namespace hw {

class elevator_controller : public interface::base_controller,
                                public ::boost::noncopyable {
public:
  explicit elevator_controller()
    : interface::base_controller("elevator-controller") {
    RCPLANE_ENTER();

    cMAX_POS = io::config_manager::instance().get<int8_t>(
      "rcplane.hw.elevator.max_pos");
    cMAX_NEG = io::config_manager::instance().get<int8_t>(
      "rcplane.hw.elevator.max_neg") * -1;
  }
  ~elevator_controller() { RCPLANE_ENTER(); }

  bool init() override {
    RCPLANE_ENTER();
    
    return true;
  }

  void start() override { RCPLANE_ENTER(); }

  void terminate() override { RCPLANE_ENTER(); }

  void on(int8_t &elevator) {
    RCPLANE_ENTER();

    if (elevator >= cMAX_POS) {
      RCPLANE_LOG(warning, _tag, "max pos hit :: " << +elevator << " -> " << +cMAX_POS);
      elevator = cMAX_POS;
    } else if (elevator <= cMAX_NEG) {
      RCPLANE_LOG(warning, _tag, "max neg hit :: " << +elevator << " -> " << +cMAX_NEG);
      elevator = cMAX_NEG;
    }
  }

private:
  int8_t cMAX_POS;
  int8_t cMAX_NEG;
};

}  // namespace hw
}  // namespace rcplane

#endif  //__RCPLANE__HW__ELEVATOR_CONTROLLER_HPP__