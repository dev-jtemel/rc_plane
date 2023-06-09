#include <sstream>
#include <iomanip>
#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/network/http_controller.hpp"

namespace rcplane {
namespace common {
namespace network {

http_controller::http_controller(std::function<void(int)> termination_handler)
  : interface::network_interface(termination_handler) {
}

http_controller::~http_controller() {
}

bool http_controller::init() {
  _svr->set_post_routing_handler([](const auto&, auto& res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "HEAD,GET,PUT,POST,DELETE,OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  });

  _svr->Get("/", [&](const httplib::Request &, httplib::Response &) {
    RCPLANE_LOG(info, _tag, "path: /");
  });

  _svr->Get("/gps", [&](const httplib::Request &, httplib::Response &res) {
    RCPLANE_LOG(info, _tag, "path: /gps");

    std::ostringstream os;
    os << std::setprecision(10);
    {
      std::lock_guard<std::mutex> lk(_gps_lk);
      os << "{\"latitude\":" << _gps.first << ",\"longitude\":" << _gps.second << "}";
    }

    res.set_content(os.str(), "application/json");
    res.status = 200;
  });

  RCPLANE_LOG(info, _tag, "initialized");
  set_state(state::initialized);
  return true;
}

void http_controller::start() {
  _worker = std::thread([&]() {
    RCPLANE_LOG(info, _tag, IP << ":" << PORT);
    _svr->listen(IP, PORT);
  });
  _worker.detach();

  RCPLANE_LOG(info, _tag, "started");
  set_state(state::running);
}

void http_controller::terminate() {
  _svr->stop();

  RCPLANE_LOG(info, _tag, "terminated");
  set_state(state::terminated);
}

} // namesapce network
} // namesapce common
} // namesapce rcplane
