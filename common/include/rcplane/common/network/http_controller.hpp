#ifndef __RCPLANE__COMMON__NETWORK__HTTP_CONTROLLER_HPP__
#define __RCPLANE__COMMON__NETWORK__HTTP_CONTROLLER_HPP__

#include "http/http.hpp"
#include "rcplane/common/network/network_interface.hpp"

namespace rcplane {
namespace common {
namespace network {

class http_controller : public interface::network_interface {
public:
  explicit http_controller(std::function<void(int)> termination_handler);
  ~http_controller();

  bool init() override;
  void start() override;
  void terminate() override;

private:
  const std::string IP = "192.168.0.30";
  const uint16_t PORT = 8080U;
  const std::unique_ptr<httplib::Server> _svr =
      std::make_unique<httplib::Server>();
};

}  // namespace network
}  // namespace common
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__NETWORK__HTTP_CONTROLLER_HPP__
