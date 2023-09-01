#include "rcplane/io/Journal.hpp"
#include "rcplane/io/telemetry/TelemetryReceiverMQ.hpp"

int main(int argc, char *argv[]) {
  RCPLANE_LOG_INIT();

  rcplane::io::telemetry::TelemetryReceiverMQ receiver;
  assert(receiver.init());

  while (true) { (void)receiver.receiveDebugMessage(); }

  return EXIT_SUCCESS;
}