#ifndef __RCPLANE__COMMON__BASE_CONTROLLER_HPP__
#define __RCPLANE__COMMON__BASE_CONTROLLER_HPP__

#include <condition_variable>
#include <mutex>
#include <string>
#include <thread>

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

protected:
  std::string _tag;
  state _state;
  std::mutex _lk;
  std::condition_variable _cv;
  volatile bool _running = false;
  std::thread _worker;
};

}  // namespace interface
}  // namespace common
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__BASE_CONTROLLER_HPP__
