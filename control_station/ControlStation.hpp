#ifndef CONTROLSTATION_HPP
#define CONTROLSTATION_HPP

#include <QLabel>
#include <QMainWindow>
#include <QThread>
#include <QVariant>
#include <memory>
#include <QGraphicsSvgItem>
#include <QGraphicsView>

#include "rcplane/io/ConfigManager.hpp"
#include "rcplane/io/telemetry/TelemetryMessage.hpp"
#include "rcplane/io/telemetry/TelemetryReceiverMQ.hpp"

Q_DECLARE_METATYPE(rcplane::io::telemetry::message::DebugMessage);
Q_DECLARE_METATYPE(rcplane::io::telemetry::message::AttitudeMessage);

QT_BEGIN_NAMESPACE
namespace Ui {
class ControlStation;
}
QT_END_NAMESPACE

class Worker : public QThread {
  Q_OBJECT
public:
  Worker(QObject *parent) : QThread(parent) {}

  void run() override {
    configManager.loadConfig("../configs/config.json");

    receiver = std::make_unique<rcplane::io::telemetry::TelemetryReceiverMQ>(
        configManager);

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
  rcplane::io::ConfigManager configManager;
  std::unique_ptr<rcplane::io::telemetry::TelemetryReceiverMQ> receiver;
};

class ControlStation : public QMainWindow {
  Q_OBJECT

public:
  ControlStation(QWidget *parent = nullptr);
  ~ControlStation();

public slots:
  void handleMessages(QVariant debug, QVariant attitude);

private:
  Ui::ControlStation *ui;
  Worker *w;
  QGraphicsScene *scene;
  QGraphicsSvgItem *svgBack;
  QGraphicsSvgItem *svgFace;
  QGraphicsSvgItem *svgRing;
  QGraphicsSvgItem *svgCase;


  QGraphicsScene *scene2;
  QGraphicsSvgItem *svgFace2;
  QGraphicsSvgItem *svgCase2;

  double oldX;
  double oldY;
};
#endif  // CONTROLSTATION_HPP
