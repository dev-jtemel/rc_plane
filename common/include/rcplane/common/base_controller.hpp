#ifndef __RCPLANE__COMMON__BASE_CONTROLLER_HPP__
#define __RCPLANE__COMMON__BASE_CONTROLLER_HPP__

#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <string>

namespace rcplane {
namespace common {
namespace interface {

class base_controller {
public:
  enum state { uninitialized, initialized, running, terminated };

  base_controller(std::string tag) : _tag(tag), _state(state::uninitialized) {}

  virtual ~base_controller() {}

  virtual bool init() = 0;
  virtual void start() = 0;
  virtual void terminate() = 0;

  void set_state(state s) { _state = s; }
  base_controller::state state() { return _state; }

protected:
  std::string _tag;
  enum state _state;
  boost::atomic<bool> _running;
  boost::thread _worker;
};

}  // namespace interface
}  // namespace common
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__BASE_CONTROLLER_HPP__
