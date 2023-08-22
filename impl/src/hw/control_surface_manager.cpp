/**
 * @file control_surface_manager.cpp
 * @author Jonathon Temelkovski (dev.jtemel@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2023-08-22
 */
#include "rcplane/hw/control_surface_manager.hpp"
#include "rcplane/io/config_manager.hpp"
#include "rcplane/io/journal.hpp"

namespace rcplane {
namespace hw {

control_surface_manager::control_surface_manager(
    rcplane::io::config_manager &config_manager)
  : interface::base_controller("control_surface_manager") {
  RCPLANE_ENTER();

  cAILERON_MAX_POS = config_manager.get<int8_t>(
      "rcplane.hw.control_surface_manager.aileron_max_pos");
  cAILERON_MAX_NEG = config_manager.get<int8_t>(
                         "rcplane.hw.control_surface_manager.aileron_max_neg")
      * -1;
  cELEVATOR_MAX_POS = config_manager.get<int8_t>(
      "rcplane.hw.control_surface_manager.elevator_max_pos");
  cELEVATOR_MAX_NEG = config_manager.get<int8_t>(
                          "rcplane.hw.control_surface_manager.elevator_max_neg")
      * -1;
  cRUDDER_MAX_POS = config_manager.get<int8_t>(
      "rcplane.hw.control_surface_manager.rudder_max_pos");
  cRUDDER_MAX_NEG = config_manager.get<int8_t>(
                        "rcplane.hw.control_surface_manager.rudder_max_neg")
      * -1;
}

control_surface_manager::~control_surface_manager() { RCPLANE_ENTER(); }

bool control_surface_manager::init() {
  RCPLANE_ENTER();
  RCPLANE_LOG(info, _tag, "initialized");
  return true;
}

void control_surface_manager::start() {
  RCPLANE_ENTER();
  RCPLANE_LOG(info, _tag, "started");
}

void control_surface_manager::terminate() {
  RCPLANE_ENTER();
  RCPLANE_LOG(info, _tag, "terminated");
}

void control_surface_manager::on(common::control_surface_packet *_cs_packet,
                                 int8_t bank) {
  RCPLANE_ENTER();

  if (bank > 0) {
    _cs_packet->aileron += 10;
  } else if (bank < 0) {
    _cs_packet->aileron -= 10;
  }

  aileron_limiter(_cs_packet->aileron);
  elevator_limiter(_cs_packet->elevator);
  rudder_limiter(_cs_packet->rudder);
}

void control_surface_manager::aileron_limiter(int8_t &aileron) {
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

void control_surface_manager::elevator_limiter(int8_t &elevator) {
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

void control_surface_manager::rudder_limiter(int8_t &rudder) {
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

}  // namespace hw
}  // namespace rcplane