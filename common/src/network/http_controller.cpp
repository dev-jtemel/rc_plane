#include "rcplane/common/network/http_controller.hpp"
#include "rcplane/common/io/journal.hpp"
#include <csignal>
#include <iomanip>
#include <sstream>

namespace rcplane {
namespace common {
namespace network {

http_controller::http_controller(std::function<void(int)> termination_handler)
  : interface::network_interface(termination_handler) {
  RCPLANE_ENTER(_tag);
}

http_controller::~http_controller() { RCPLANE_ENTER(_tag); }

bool http_controller::init() {
  RCPLANE_ENTER(_tag);

  _curl = curl_easy_init();
  if (!_curl) { return false; }

  _svr->set_post_routing_handler([](const auto &, auto &res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods",
                   "HEAD,GET,PUT,POST,DELETE,OPTIONS");
    res.set_header("Access-Control-Allow-Headers",
                   "Origin, X-Requested-With, Content-Type, Accept");
  });

  _svr->Get("/", [&](const httplib::Request &, httplib::Response &) {
    RCPLANE_LOG(debug, _tag, "path: /");
  });

  _svr->Get("/stop", [&](const httplib::Request &, httplib::Response &) {
    RCPLANE_LOG(debug, _tag, "path: /stop");
    _svr->stop();
  });

  _svr->Get("/gps", [&](const httplib::Request &, httplib::Response &res) {
    RCPLANE_LOG(debug, _tag, "path: /gps");

    std::ostringstream os;
    os << std::setprecision(10);
    {
      std::lock_guard<std::mutex> lk(_gps_lk);
      os << "{\"latitude\":" << std::get<0>(_gps)
         << ",\"longitude\":" << std::get<1>(_gps)
         << ",\"track\":" << std::get<2>(_gps)
         << ",\"speed\":" << std::get<3>(_gps) << "}";
    }

    res.set_content(os.str(), "application/json");
    res.status = 200;
  });

  _svr->Get("/cs", [&](const httplib::Request &, httplib::Response &res) {
    RCPLANE_LOG(debug, _tag, "path: /cs");

    std::ostringstream os;
    os << std::setprecision(10);
    {
      std::lock_guard<std::mutex> lk(_cs_lk);
      os << "{\"state\":" << +std::get<0>(_cs)
         << ",\"motor\":" << +std::get<1>(_cs)
         << ",\"aileron\":" << +std::get<2>(_cs)
         << ",\"elevator\":" << +std::get<3>(_cs)
         << ",\"rudder\":" << +std::get<4>(_cs) << "}";
    }

    res.set_content(os.str(), "application/json");
    res.status = 200;
  });

  _svr->Get("/gyro", [&](const httplib::Request &, httplib::Response &res) {
    RCPLANE_LOG(debug, _tag, "path: /gyro");

    std::ostringstream os;
    os << std::setprecision(10);
    {
      std::lock_guard<std::mutex> lk(_gyro_lk);
      os << "{\"pitch\":" << +std::get<0>(_gyro)
         << ",\"roll\":" << +std::get<1>(_gyro)
         << ",\"yaw\":" << +std::get<2>(_gyro) << "}";
    }

    res.set_content(os.str(), "application/json");
    res.status = 200;
  });

  RCPLANE_LOG(info, _tag, "initialized");
  set_state(state::initialized);
  return true;
}

void http_controller::start() {
  RCPLANE_ENTER(_tag);

  RCPLANE_LOG(info, _tag, IP << ":" << PORT);
  _worker = boost::thread([&]() {
    _svr->listen(IP, PORT);
  });

  RCPLANE_LOG(info, _tag, "started");
  set_state(state::running);
}

void http_controller::terminate() {
  RCPLANE_ENTER(_tag);

  curl_easy_setopt(_curl, CURLOPT_URL, "http://localhost:8080/stop");
  curl_easy_perform(_curl);
  curl_easy_cleanup(_curl);
  _worker.join();

  RCPLANE_LOG(info, _tag, "terminated");
  set_state(state::terminated);
}

}  // namespace network
}  // namespace common
}  // namespace rcplane
