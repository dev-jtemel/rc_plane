#ifndef __RCPLANE__COMMON__NETWORK__HTTP_CONTROLLER_HPP__
#define __RCPLANE__COMMON__NETWORK__HTTP_CONTROLLER_HPP__

#include "http/http.hpp"
#include "rcplane/common/network/network_interface.hpp"
#include <curl/curl.h>
#include <memory>

namespace rcplane {
namespace common {
namespace network {

/**
* @brief Control and host a HTTP server.
*/
class http_controller : public interface::network_interface {
public:
  /**
   * @param termination_handler Hanlder to fire on termination.
   */
  explicit http_controller(std::function<void(int)> termination_handler);
  ~http_controller();

  /**
   * @brief Initialize the http server.
   *
   * Handles registering paths and API to expose over https.
   *
   * @return Status of the initialization.
   */
  bool init() override;

  /**
   * @brief Start the HTTP server.
   * @pre init()
   */
  void start() override;

  /**
   * @brief Terminate the HTTP server.
   * @pre start()
   * @warning Sends a CURL request /stop to internally destruct the underlying server.
   */
  void terminate() override;

private:
  const std::string IP = "localhost";
  const uint16_t PORT = 8080U;
  const std::unique_ptr<httplib::Server> _svr =
      std::make_unique<httplib::Server>();
  CURL *_curl;
};

}  // namespace network
}  // namespace common
}  // namespace rcplane

#endif  //__RCPLANE__COMMON__NETWORK__HTTP_CONTROLLER_HPP__
