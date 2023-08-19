#ifndef __RCPLANE__HW__CONTROL_SURFACE_MANAGER_HPP__
#define __RCPLANE__HW__CONTROL_SURFACE_MANAGER_HPP__

#include <boost/noncopyable.hpp>

#include "rcplane/base_controller.hpp"
#include "rcplane/common/packet.hpp"

namespace rcplane {
namespace hw {

/**
 * @brief Manages and controls the control surfaces on the plane.
 * 
 * The control surfaces (ailerons, elevators and rudder) are controlled
 * and maintained here.
  */
class control_surface_manager : public interface::base_controller,
                                public ::boost::noncopyable {
public:
  /**
   * @brief Construct a new control surface manager object.
   */
  explicit control_surface_manager();
  ~control_surface_manager();

  /**
   * @brief NOP.
   * @return true always.
   */
  bool init() override;

  /**
   * @brief NOP.
   */
  void start() override;

  /**
   * @brief NOP.
   */
  void terminate() override;

  /**
   * @brief Handle an incoming control_surface_packet, applying
   * and filters and/or limiters.
   * 
   * @param _cs_packet Pointer to the most recent packet received.
   */
  void on(common::control_surface_packet *_cs_packet);

private:
  /**
   * @brief Limit the aileron deflection within the configured bounds.
   * 
   * @param aileron Aileron ref in the current packet.
   */
  void aileron_limiter(int8_t &aileron);

  /**
   * @brief Limit the elevator deflection within the configured bounds.
   * 
   * @param elevator Elevator ref in the current packet.
   */
  void elevator_limiter(int8_t &elevator);

  /**
   * @brief Limit the rudder deflection within the configured bounds.
   * 
   * @param rudder Rudder ref in the current packet.
   */
  void rudder_limiter(int8_t &rudder);

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