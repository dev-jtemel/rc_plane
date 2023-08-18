/** 
 * @file base_controller.hpp
 * @author Jonathon Temelkovski <dev.jtemel@gmail.com>
 */
#ifndef __RCPLANE__BASE_CONTROLLER_HPP__
#define __RCPLANE__BASE_CONTROLLER_HPP__

#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <string>

namespace rcplane {
namespace interface {

/**
 * @brief Base class for all controllers to be dervied from.
 *
 * This class maintains and exposes the lifecycle of controllers. All controllers
 * should inherit from this class to provide polymorphism.
 */
class base_controller {
public:
  /**
   * @brief Possible states.
   */
  enum class state : uint8_t {
    uninitialized = 0,
    initialized = 1,
    running = 2,
    terminated = 3
  };

  /**
   * @param tag The logging tag to be prepend to internally produced log statements.
   */
  explicit base_controller(std::string tag)
    : _tag(tag), _state(state::uninitialized) {}
  virtual ~base_controller() {}

  /**
   * @brief Initialize the controller.
   *
   * Initalize the controller and perform any setup procedures required.
   *
   * @returns True if the initialization was successful, false otherwise.
   */
  virtual bool init() = 0;

  /**
   * @brief Start the controller.
   *
   * Start the execution of the controller. This may spawn a worker thread internally.
   *
   * @pre init() was called and completed successfully.
   */
  virtual void start() = 0;

  /**
   * @brief Terminate the controller.
   *
   * Request to stop the execution of the controller. This may block until if a thread is
   * executing and awaiting completion.
   *
   * @pre start() was called.
   */
  virtual void terminate() = 0;

  /**
   * @brief Set the state of the controller.
   *
   * @param s The state to transition to.
   */
  void set_state(state s) { _state = s; }

  /**
   * @return The current state of the controller.
   */
  base_controller::state state() const { return _state; }

protected:
  std::string _tag;
  enum state _state;
};

}  // namespace interface
}  // namespace rcplane

#endif  //__RCPLANE__BASE_CONTROLLER_HPP__
