#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/Journal.hpp"
#include "rcplane/io/telemetry/TelemetryReceiverMQ.hpp"

int main(int argc, char *argv[]) {
  RCPLANE_LOG_INIT();

  rcplane::io::ConfigManager configManager;
  assert(configManager.loadConfig());

  rcplane::io::telemetry::TelemetryReceiverMQ receiver(configManager);
  assert(receiver.init());

  while (true) {
    while (!receiver.receiveDebugMessage())
      ;
    while (!receiver.receiveAttitudeMessage())
      ;
  }

  return EXIT_SUCCESS;
}