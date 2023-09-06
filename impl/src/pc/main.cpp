#include "rcplane/common/Helpers.hpp"
#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/Journal.hpp"
#include "rcplane/io/telemetry/TelemetryReceiverMQ.hpp"

int main(int argc, char *argv[]) {
  RCPLANE_LOG_INIT();

  rcplane::io::ConfigManager configManager;
  RCPLANE_ASSERT_EXTERNAL(configManager.loadConfig(), "main");

  rcplane::io::telemetry::TelemetryReceiverMQ receiver(configManager);
  RCPLANE_ASSERT_EXTERNAL(receiver.init(), "main");

  while (true) {
    rcplane::io::telemetry::message::DebugMessage debugMessage;
    rcplane::io::telemetry::message::AttitudeMessage attitudeMessage;
    while (!receiver.receiveDebugMessage(debugMessage))
      ;
    while (!receiver.receiveAttitudeMessage(attitudeMessage))
      ;
  }

  return EXIT_SUCCESS;
}