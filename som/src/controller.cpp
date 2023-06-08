#include <csignal>
#include <bitset>
#include <mutex>
#include <condition_variable>
#include <string>
#include <memory>

#include "rcplane/common/io/journal.hpp"
#include "rcplane/common/io/serial.hpp"
#include "rcplane/common/network/http_controller.hpp"

std::string TAG = "som-controller";

std::mutex main_lock;
std::condition_variable main_cv;
volatile bool running = false;

void termination_handler(int signum) {
  RCPLANE_LOG(warn, TAG, "\ntermination signal received");

  std::lock_guard<std::mutex> lock(main_lock);
  running = false;
  main_cv.notify_one();
}

int main(int argc, char *argv[]) {
  RCPLANE_SEVERITY(trace);
  RCPLANE_LOG(info, TAG, "starting");

  signal(SIGINT, termination_handler);
  RCPLANE_LOG(info, TAG, "termination handler set");

  // Create network controller
  std::unique_ptr<rcplane::common::network::interface::network_interface> network_controller
    = std::make_unique<rcplane::common::network::http_controller>(
        termination_handler
      );

  // Create and link controllers
  std::vector<std::unique_ptr<rcplane::common::interface::base_controller>> controllers;

  auto serial_controller = std::make_unique<rcplane::common::io::serial>();
  serial_controller->register_cb([&](auto timestamp, auto &packets){
    RCPLANE_LOG(debug, TAG, "serial_controller cb fired!");
  });
  controllers.push_back(std::move(serial_controller));

  // Initialize
  network_controller->init();
  for (auto &c : controllers) {
    if (!c->init()) {
      return EXIT_FAILURE;
    }
  }

  // Start the controllers
  network_controller->start();
  for (auto &c : controllers) {
    c->start();
  }

  {
    std::unique_lock<std::mutex> lock(main_lock);
    running = true;
    main_cv.wait(lock, [&]{ return !running; });
  }

  // Stop the controllers
  for (auto &c : controllers) {
    c->terminate();
  }
  network_controller->terminate();

  return EXIT_SUCCESS;
}
