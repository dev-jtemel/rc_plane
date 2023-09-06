#include "ControlStation.hpp"
#include "./ui_controlstation.h"
#include <QObject>
#include <QThread>
#include <QGridLayout>

ControlStation::ControlStation(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::ControlStation) {
  ui->setupUi(this);

  w = new Worker(this);
  w->start();

  connect(w,
          &Worker::sendMessages,
          this,
          &ControlStation::handleMessages);
}

ControlStation::~ControlStation() { delete ui; }

void ControlStation::handleMessages(QVariant debug, QVariant attitude) {
  rcplane::io::telemetry::message::DebugMessage message =
      debug.value<rcplane::io::telemetry::message::DebugMessage>();
  ui->mlCount->setText(QString::number(message.mainLoopCounter));
  ui->txOK->setText(QString::number(message.serialWrites));
  ui->txTO->setText(QString::number(message.serialWriteTimeouts));
  ui->rxOK->setText(QString::number(message.serialReads));
  ui->rxTO->setText(QString::number(message.serialReadTimeouts));
}
