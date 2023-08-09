#include <bitset>
#include <condition_variable>
#include <csignal>
#include <memory>
#include <mutex>
#include <string>

#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/serial_controller.hpp"
#include "rcplane/common/network/http_controller.hpp"
#include "rcplane/som/position/gps_controller.hpp"

std::string TAG = "som-controller";

std::mutex main_lock;
std::condition_variable main_cv;
volatile bool running = false;

void termination_handler(int signum) {
  RCPLANE_LOG(warning, TAG, "termination signal received");

  std::lock_guard<std::mutex> lock(main_lock);
  running = false;
  main_cv.notify_all();
}

int main(int argc, char *argv[]) {
  RCPLANE_SEVERITY(trace);
  RCPLANE_LOG(info, TAG, "starting");

  signal(SIGINT, termination_handler);
  RCPLANE_LOG(info, TAG, "termination handler set");

  // Create network controller
  std::unique_ptr<rcplane::common::network::interface::network_interface>
      network_controller =
          std::make_unique<rcplane::common::network::http_controller>(
              termination_handler);

  // Create and link controllers
  std::vector<std::unique_ptr<rcplane::common::interface::base_controller>>
      controllers;

  auto serial_controller =
      std::make_unique<rcplane::common::io::serial_controller>();
  serial_controller->register_cs_cb(
      std::bind(&rcplane::common::network::interface::network_interface::cs_cb,
                network_controller.get(),
                std::placeholders::_1,
                std::placeholders::_2,
                std::placeholders::_3,
                std::placeholders::_4,
                std::placeholders::_5));
  serial_controller->register_gyro_cb(std::bind(
      &rcplane::common::network::interface::network_interface::gyro_cb,
      network_controller.get(),
      std::placeholders::_1,
      std::placeholders::_2,
      std::placeholders::_3));
  controllers.push_back(std::move(serial_controller));

  auto gps_controller =
      std::make_unique<rcplane::som::position::gps_controller>();
  gps_controller->register_cb(
      std::bind(&rcplane::common::network::interface::network_interface::gps_cb,
                network_controller.get(),
                std::placeholders::_1,
                std::placeholders::_2,
                std::placeholders::_3,
                std::placeholders::_4));
  controllers.push_back(std::move(gps_controller));

  // Initialize
  network_controller->init();
  for (auto &c : controllers) {
    if (!c->init()) { return EXIT_FAILURE; }
  }

  // Start the controllers
  network_controller->start();
  for (auto &c : controllers) { c->start(); }

  {
    std::unique_lock<std::mutex> lock(main_lock);
    running = true;
    main_cv.wait(lock, [&] { return !running; });
  }

  // Stop the controllers
  for (auto &c : controllers) { c->terminate(); }
  network_controller->terminate();

  return EXIT_SUCCESS;
}
