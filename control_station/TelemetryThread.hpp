#ifndef TELEMETRYTHREAD_HPP
#define TELEMETRYTHREAD_HPP

#include <QThread>
#include <QVariant>

#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/telemetry/TelemetryMessage.hpp"
#include "rcplane/io/telemetry/TelemetryReceiverMQ.hpp"

Q_DECLARE_METATYPE(rcplane::io::telemetry::message::DebugMessage);
Q_DECLARE_METATYPE(rcplane::io::telemetry::message::AttitudeMessage);

class TelemetryThread : public QThread {
  Q_OBJECT
public:
  TelemetryThread(rcplane::io::ConfigManager &configManager, QObject *parent) : QThread(parent) {

    receiver = std::make_unique<rcplane::io::telemetry::TelemetryReceiverMQ>(
        configManager);
  }

  void run() override {
    receiver->init();

    forever {
      rcplane::io::telemetry::message::DebugMessage debugMsg;
      receiver->receiveDebugMessage(debugMsg);

      rcplane::io::telemetry::message::AttitudeMessage attitudeMsg;
      receiver->receiveAttitudeMessage(attitudeMsg);

      QVariant debugMessageVariant;
      debugMessageVariant.setValue(debugMsg);

      QVariant attitudeMessageVariant;
      attitudeMessageVariant.setValue(attitudeMsg);

      emit sendMessages(debugMessageVariant, attitudeMessageVariant);
    }
  }

signals:
  void sendMessages(QVariant debug, QVariant attitude);

private:
  std::unique_ptr<rcplane::io::telemetry::TelemetryReceiverMQ> receiver;
};

#endif // TELEMETRYTHREAD_HPP
