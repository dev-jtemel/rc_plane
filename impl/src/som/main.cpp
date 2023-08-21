#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_service.hpp>
#include <csignal>
#include <memory>

#include "rcplane/autopilot/autopilot_manager.hpp"
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
  worker.~executor_work_guard();
  worker.~executor_work_guard();
}

int main() {
  RCPLANE_LOG_INIT();
  RCPLANE_LOG(info, kTAG, "starting");

  signal(SIGINT, termination_handler);
  RCPLANE_LOG(info, kTAG, "termination handler set");

  std::unique_ptr<rcplane::io::serial_controller> serial_controller =
      std::make_unique<rcplane::io::serial_controller>(io);

  std::unique_ptr<rcplane::autopilot::autopilot_manager> autopilot_manager =
      std::make_unique<rcplane::autopilot::autopilot_manager>(io);

  serial_controller->state_signal().connect(
      boost::bind(&rcplane::autopilot::autopilot_manager::on_state_signal,
                  autopilot_manager.get(),
                  boost::placeholders::_1));
  serial_controller->packet_signal().connect(
      boost::bind(&rcplane::autopilot::autopilot_manager::on_mcu_signal,
                  autopilot_manager.get(),
                  boost::placeholders::_1,
                  boost::placeholders::_2));

  autopilot_manager->init();
  serial_controller->init();



  autopilot_manager->autopilot()->write_signal().connect(
      boost::bind(&rcplane::io::serial_controller::on_write_signal,
                  serial_controller.get()));

  autopilot_manager->start();
  serial_controller->start();

  io.run();

  serial_controller->terminate();
  autopilot_manager->terminate();

  return 0;
}
