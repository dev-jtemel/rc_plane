#ifndef __RCPLANE__HW__CONTROL_SURFACE_MANAGER_HPP__
#define __RCPLANE__HW__CONTROL_SURFACE_MANAGER_HPP__

#include <boost/noncopyable.hpp>

#include "rcplane/base_controller.hpp"
#include "rcplane/common/packet.hpp"
#include "rcplane/io/config_manager.hpp"
#include "rcplane/io/journal.hpp"

namespace rcplane {
namespace hw {

class control_surface_manager : public interface::base_controller,
                                public ::boost::noncopyable {
public:
  explicit control_surface_manager()
    : interface::base_controller("cs-manager") {
    RCPLANE_ENTER();

    cAILERON_MAX_POS = io::config_manager::instance().get<int8_t>(
        "rcplane.hw.aileron.max_pos");
    cAILERON_MAX_NEG =
        io::config_manager::instance().get<int8_t>("rcplane.hw.aileron.max_neg")
        * -1;
    cELEVATOR_MAX_POS = io::config_manager::instance().get<int8_t>(
        "rcplane.hw.elevator.max_pos");
    cELEVATOR_MAX_NEG = io::config_manager::instance().get<int8_t>(
                            "rcplane.hw.elevator.max_neg")
        * -1;
    cRUDDER_MAX_POS =
        io::config_manager::instance().get<int8_t>("rcplane.hw.rudder.max_pos");
    cRUDDER_MAX_NEG =
        io::config_manager::instance().get<int8_t>("rcplane.hw.rudder.max_neg")
        * -1;
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

    aileron_limiter(_cs_packet->aileron);
    elevator_limiter(_cs_packet->elevator);
    rudder_limiter(_cs_packet->rudder);
  }

private:
  void aileron_limiter(int8_t &aileron) {
    if (aileron >= cAILERON_MAX_POS) {
      RCPLANE_LOG(debug,
                  _tag,
                  "aileron :: max pos hit :: " << +aileron << " -> "
                                               << +cAILERON_MAX_POS);
      aileron = cAILERON_MAX_POS;
    } else if (aileron <= cAILERON_MAX_NEG) {
      RCPLANE_LOG(debug,
                  _tag,
                  "aileron :: max neg hit :: " << +aileron << " -> "
                                               << +cAILERON_MAX_NEG);
      aileron = cAILERON_MAX_NEG;
    }
  }

  void elevator_limiter(int8_t &elevator) {
    if (elevator >= cELEVATOR_MAX_POS) {
      RCPLANE_LOG(debug,
                  _tag,
                  "elevator :: max pos hit :: " << +elevator << " -> "
                                                << +cELEVATOR_MAX_POS);
      elevator = cELEVATOR_MAX_POS;
    } else if (elevator <= cELEVATOR_MAX_NEG) {
      RCPLANE_LOG(debug,
                  _tag,
                  "elevator :: max neg hit :: " << +elevator << " -> "
                                                << +cELEVATOR_MAX_NEG);
      elevator = cELEVATOR_MAX_NEG;
    }
  }

  void rudder_limiter(int8_t &rudder) {
    if (rudder >= cRUDDER_MAX_POS) {
      RCPLANE_LOG(debug,
                  _tag,
                  "rudder :: max pos hit :: " << +rudder << " -> "
                                              << +cRUDDER_MAX_POS);
      rudder = cRUDDER_MAX_POS;
    } else if (rudder <= cRUDDER_MAX_NEG) {
      RCPLANE_LOG(debug,
                  _tag,
                  "rudder :: max neg hit :: " << +rudder << " -> "
                                              << +cRUDDER_MAX_NEG);
      rudder = cRUDDER_MAX_NEG;
    }
  }

  int8_t cAILERON_MAX_POS{};
  int8_t cAILERON_MAX_NEG{};
  int8_t cELEVATOR_MAX_POS{};
  int8_t cELEVATOR_MAX_NEG{};
  int8_t cRUDDER_MAX_POS{};
  int8_t cRUDDER_MAX_NEG{};
};

}  // namespace hw
}  // namespace rcplane

#endif  //__RCPLANE__HW__CONTROL_SURFACE_MANAGER_HPP__