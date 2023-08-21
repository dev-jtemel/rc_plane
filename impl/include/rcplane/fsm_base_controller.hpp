/**
 * @file fsm_base_controller.hpp
 * @author Jonathon Temelkovski (dev.jtemel@gmail.com)
 * @version 0.1
 * @date 2023-08-21
 */
#ifndef __RCPLANE__FSM_BASE_CONTROLLER_HPP__
#define __RCPLANE__FSM_BASE_CONTROLLER_HPP__

#include <string>

namespace rcplane {
namespace interface {

/**
 * @brief Base class for all fsm controllers to be dervied from.
 *
 * This class maintains and exposes the lifecycle of controllers. All controllers
 * should inherit from this class to conform to correct transition fsm behaviour.
 */
class fsm_base_controller {
public:
  /**
   * @param tag The logging tag to be prepend to internally produced log statements.
   */
  explicit fsm_base_controller(std::string tag) : _tag(tag) {}
  virtual ~fsm_base_controller() {}

  /**
   * @brief Pre condition to fire when entering this state.
   */
  virtual void on_entry() = 0;

  /**
   * @brief Pre condition to fire when exiting this state.
   */
  virtual void on_exit() = 0;

protected:
  std::string _tag;
};

}  // namespace interface
}  // namespace rcplane

#endif  //__RCPLANE__FSM_BASE_CONTROLLER_HPP__