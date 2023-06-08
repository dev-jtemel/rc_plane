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
  RCPLANE_LOG(warn, TAG, "termination signal received");

  std::lock_guard<std::mutex> lock(main_lock);
  running = false;
  main_cv.notify_one();
}

int main(int argc, char *argv[]) {
  RCPLANE_SEVERITY(trace);
  RCPLANE_LOG(info, TAG, "starting");

  signal(SIGINT, termination_handler);
  RCPLANE_LOG(info, TAG, "termination handler set");

  std::function<void()> start_handler = [&](){};
  std::function<void()> stop_handler = [&](){};

  std::unique_ptr<rcplane::common::network::interface::network_interface> network_controller
    = std::make_unique<rcplane::common::network::http_controller>(
        termination_handler
      );

  network_controller->register_start_handler(start_handler);
  network_controller->register_stop_handler(stop_handler);

  std::unique_ptr<rcplane::common::io::serial> serial_controller
    = std::make_unique<rcplane::common::io::serial>();

  serial_controller->register_cb([&](auto timestamp, auto &packets){
    RCPLANE_LOG(
      info,
      TAG,
      "[" << timestamp << "]"
      << " state = " << std::bitset<8>(packets[0].data())
      << " | motor = " << packets[1].data()
      << " | aileron = " << packets[2].data()
      << " | elevator = " << packets[3].data()
      << " | rudder = " << packets[4].data()
    ); 
  });

  network_controller->init();
  network_controller->start();

  serial_controller->read_serial();

  {
    std::unique_lock<std::mutex> lock(main_lock);
    running = true;
    main_cv.wait(lock, [&]{ return !running; });
  }

  network_controller->terminate();

  return EXIT_SUCCESS;
}
