#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_service.hpp>
#include <chrono>  //rm
#include <csignal>
#include <iomanip>  //rm
#include <memory>
#include <thread>  //rm

#include "rcplane/io/config_manager.hpp"
#include "rcplane/io/journal.hpp"
#include "rcplane/io/serial_controller.hpp"

const std::string kTAG = "som-controller";
boost::asio::io_service io;
boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
    worker = boost::asio::make_work_guard(io);

void termination_handler(int signum) {
  RCPLANE_LOG(warning, kTAG, "termination signal received");
  worker.reset();
  io.stop();
  io.reset();
}

struct fast {
  void timestamp(uint32_t timestamp) {
    RCPLANE_LOG(warning, "slower", timestamp);
  }
};

struct slow {
  void timestamp(uint32_t timestamp) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    RCPLANE_LOG(warning, "slow", timestamp);
  }
};

struct coml {
  void timestamp(uint32_t timestamp) {
    RCPLANE_LOG(error, "done", std::hex << timestamp << std::dec);
    done();
  }

  boost::signals2::signal<void()> done;
};

int main() {
  RCPLANE_LOG_INIT();
  RCPLANE_LOG(info, kTAG, "starting");

  signal(SIGINT, termination_handler);
  RCPLANE_LOG(info, kTAG, "termination handler set");

  std::unique_ptr<rcplane::io::serial_controller> serial_controller =
      std::make_unique<rcplane::io::serial_controller>(io);

  fast f;
  slow s;
  coml c;
  serial_controller->signals().connect(
      boost::bind(&fast::timestamp, boost::ref(f), boost::placeholders::_1));
  serial_controller->signals().connect(
      boost::bind(&slow::timestamp, boost::ref(s), boost::placeholders::_1));
  serial_controller->signals().connect(
      boost::bind(&coml::timestamp, boost::ref(c), boost::placeholders::_1));

  c.done.connect(boost::bind(&rcplane::io::serial_controller::write,
                             serial_controller.get()));

  serial_controller->init();
  serial_controller->start();

  io.run();

  serial_controller->terminate();

  return 1;
}
