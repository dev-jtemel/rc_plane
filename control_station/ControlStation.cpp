#include "ControlStation.hpp"
#include "./ui_controlstation.h"
#include <QObject>
#include <QThread>
#include <QGridLayout>

#include "rcplane/io/telemetry/TelemetryMessage.hpp"

Q_DECLARE_METATYPE(rcplane::io::telemetry::message::DebugMessage);
Q_DECLARE_METATYPE(rcplane::io::telemetry::message::AttitudeMessage);

ControlStation::ControlStation(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::ControlStation) {

  ui->setupUi(this);

  m_attitudeIndicator = new AttitudeIndicator(ui->gw);
  m_headingIndicator = new HeadingIndicator(ui->gw2);
}

ControlStation::~ControlStation() {
  delete ui;
}

void ControlStation::handleMessages(QVariant debug, QVariant attitude) {
  rcplane::io::telemetry::message::DebugMessage debugMessage =
      debug.value<rcplane::io::telemetry::message::DebugMessage>();
  ui->mlCount->setText(QString::number(debugMessage.mainLoopCounter));
  ui->txOK->setText(QString::number(debugMessage.serialWrites));
  ui->txTO->setText(QString::number(debugMessage.serialWriteTimeouts));
  ui->rxOK->setText(QString::number(debugMessage.serialReads));
  ui->rxTO->setText(QString::number(debugMessage.serialReadTimeouts));

  rcplane::io::telemetry::message::AttitudeMessage attitudeMessage =
      attitude.value<rcplane::io::telemetry::message::AttitudeMessage>();

  m_attitudeIndicator->setAttitude(attitudeMessage.rollAngle, attitudeMessage.pitchAngle);
  m_headingIndicator->setHeading(attitudeMessage.yawAngle);
}
