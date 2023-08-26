#include <chrono>
#include <thread>

#include "rcplane/io/Journal.hpp"
#include "rcplane/som/SomController.hpp"

int main(int argc, char *argv[]) {
  RCPLANE_LOG_INIT();
  rcplane::som::SomController somController;
  somController.startIoThread();

  if (!somController.handshakeMCU()) { return EXIT_FAILURE; }

  somController.runMainLoop();
  return EXIT_SUCCESS;
}