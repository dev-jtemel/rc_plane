#include "ControlStation.hpp"

#include <QApplication>

#include "TelemetryThread.hpp"
#include "rcplane/io/ConfigManager.hpp"

int main(int argc, char *argv[]) {
  rcplane::io::ConfigManager configManager;
  configManager.loadConfig("../configs/config.json");

  QApplication a(argc, argv);
  ControlStation window;
  TelemetryThread telemetryThread(configManager, &window);

  QObject::connect(&telemetryThread,
          &TelemetryThread::sendMessages,
          &window,
          &ControlStation::handleMessages);

  window.show();
  telemetryThread.start();
  return a.exec();
}
