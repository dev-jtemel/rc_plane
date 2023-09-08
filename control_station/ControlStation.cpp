#include "ControlStation.hpp"
#include "./ui_controlstation.h"
#include <QObject>
#include <QThread>
#include <QGridLayout>

#include "rcplane/io/telemetry/TelemetryMessage.hpp"

Q_DECLARE_METATYPE(rcplane::io::telemetry::message::DebugMessage);
Q_DECLARE_METATYPE(rcplane::io::telemetry::message::AttitudeMessage);
Q_DECLARE_METATYPE(rcplane::io::telemetry::message::OnboardStateMessage);

ControlStation::ControlStation(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::ControlStation) {

  ui->setupUi(this);

  m_attitudeIndicator = new AttitudeIndicator(ui->gw);
  m_headingIndicator = new HeadingIndicator(ui->gw2);
}

ControlStation::~ControlStation() {
  delete ui;
}

void ControlStation::handleMessages(QVariant debug, QVariant attitude, QVariant onboard) {
  rcplane::io::telemetry::message::DebugMessage debugMessage =
      debug.value<rcplane::io::telemetry::message::DebugMessage>();
  ui->mlCount->setText(QString::number(debugMessage.mainLoopCounter));
  ui->txOK->setText(QString::number(debugMessage.serialWrites));
  ui->txTO->setText(QString::number(debugMessage.serialWriteTimeouts));
  ui->rxOK->setText(QString::number(debugMessage.serialReads));
  ui->rxTO->setText(QString::number(debugMessage.serialReadTimeouts));

  rcplane::io::telemetry::message::OnboardStateMessage onboardMessage =
      onboard.value<rcplane::io::telemetry::message::OnboardStateMessage>();

  ui->mtm->setText(QString::number(onboardMessage.mcuTimestamp));
  ui->thl->setText(QString::number(onboardMessage.throttle));
  ui->adf->setText(QString::number(onboardMessage.aileronDeflection));
  ui->edf->setText(QString::number(onboardMessage.elevatorDeflection));
  ui->rdf->setText(QString::number(onboardMessage.rudderDeflection));
  ui->apt->setText(QString::fromStdString(rcplane::io::telemetry::message::gAutopilotTypeToStr.at(onboardMessage.autopilotType)));

  rcplane::io::telemetry::message::AttitudeMessage attitudeMessage =
      attitude.value<rcplane::io::telemetry::message::AttitudeMessage>();

  ui->chd->setText(QString::number(attitudeMessage.courseHeading));

  m_attitudeIndicator->setAttitude(attitudeMessage.rollAngle, attitudeMessage.pitchAngle);
  m_headingIndicator->setHeading(attitudeMessage.yawAngle);

}
