#include "ControlStation.hpp"
#include "./ui_controlstation.h"
#include <QObject>
#include <QThread>
#include <QGridLayout>

ControlStation::ControlStation(QWidget *parent)
  : QMainWindow(parent), ui(new Ui::ControlStation) {
  ui->setupUi(this);

  scene = new QGraphicsScene(this);
  svgBack = new QGraphicsSvgItem( "/tmp/images/ai/ai_back.svg" );
  svgBack->setCacheMode( QGraphicsItem::NoCache );
  svgBack->setZValue( -30 );
  svgBack->setTransform( QTransform::fromScale( 1, 1), true );
  svgBack->setTransformOriginPoint( 120,120 );
  scene->addItem( svgBack );

  svgFace = new QGraphicsSvgItem( "/tmp/images/ai/ai_face.svg" );
  svgFace->setCacheMode( QGraphicsItem::NoCache );
  svgFace->setZValue( -20 );
  svgFace->setTransform( QTransform::fromScale( 1,1 ), true );
  svgFace->setTransformOriginPoint(  120,120 );
  scene->addItem( svgFace );

  svgRing = new QGraphicsSvgItem( "/tmp/images/ai/ai_ring.svg" );
  svgRing->setCacheMode( QGraphicsItem::NoCache );
  svgRing->setZValue( -10 );
  svgRing->setTransform( QTransform::fromScale( 1,1 ), true );
  svgRing->setTransformOriginPoint(  120,120 );
  scene->addItem( svgRing );

  svgCase = new QGraphicsSvgItem( "/tmp/images/ai/ai_case.svg" );
  svgCase->setCacheMode( QGraphicsItem::NoCache );
  svgCase->setZValue( 10 );
  svgCase->setTransform( QTransform::fromScale( 1,1 ), true );
  scene->addItem( svgCase );

  ui->gw->setScene(scene);
  ui->gw->show();

  ui->gw->setBackgroundBrush(Qt::black);

  scene2 = new QGraphicsScene(this);
  svgFace2 = new QGraphicsSvgItem( "/tmp/images/hi/hi_face.svg" );
  svgFace2->setCacheMode( QGraphicsItem::NoCache );
  svgFace2->setZValue( -20 );
  svgFace2->setTransform( QTransform::fromScale( 1,1 ), true );
  svgFace2->setTransformOriginPoint(  120,120 );
  scene2->addItem( svgFace2 );

  svgCase2 = new QGraphicsSvgItem( "/tmp/images/hi/hi_case.svg" );
  svgCase2->setCacheMode( QGraphicsItem::NoCache );
  svgCase2->setZValue( 10 );
  svgCase2->setTransform( QTransform::fromScale( 1,1 ), true );
  scene2->addItem( svgCase2 );

  ui->gw2->setScene(scene2);
  ui->gw2->show();
  ui->gw2->setBackgroundBrush(Qt::black);


  w = new Worker(this);
  w->start();

  connect(w,
          &Worker::sendMessages,
          this,
          &ControlStation::handleMessages);
}

ControlStation::~ControlStation() {
  delete scene;
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

  double roll = attitudeMessage.rollAngle;
  double rollRad = M_PI * roll / 180.0;
  double pitch = attitudeMessage.pitchAngle;

  if (pitch > 30) {
    pitch = 30;
  } else if (pitch < -30){
    pitch = -30;
  }
  double delta = pitch * 1.7;

  svgBack->setRotation(-roll);
  svgFace->setRotation(-roll);
  svgRing->setRotation(-roll);

  double newX = delta * sin(rollRad);
  double newY = delta * cos(rollRad);

  svgFace->moveBy(newX - oldX, -1 * (newY - oldY));
  svgFace2->setRotation(attitudeMessage.yawAngle);

  oldY = newY;
  oldX = newX;
}
