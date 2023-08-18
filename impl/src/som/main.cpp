#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_service.hpp>
#include <csignal>
#include <memory>

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

int main() {
  RCPLANE_LOG_INIT();
  RCPLANE_LOG(info, kTAG, "starting");

  signal(SIGINT, termination_handler);
  RCPLANE_LOG(info, kTAG, "termination handler set");

  std::unique_ptr<rcplane::io::serial_controller> serial_controller =
      std::make_unique<rcplane::io::serial_controller>(io);

  serial_controller->init();
  serial_controller->start();

  io.run();

  serial_controller->terminate();

  return 1;
}