#ifndef ATTITUDEINDICATOR_HPP
#define ATTITUDEINDICATOR_HPP

#include <QGraphicsSvgItem>
#include <QGraphicsView>
#include <QtMath>

class AttitudeIndicator : public QGraphicsView {
  Q_OBJECT

public:
  explicit AttitudeIndicator(QWidget *parent = Q_NULLPTR) : QGraphicsView(parent) {
    m_scene = new QGraphicsScene(this);

    m_svgBack = new QGraphicsSvgItem( "/tmp/images/ai/ai_back.svg" );
    m_svgBack->setCacheMode( QGraphicsItem::NoCache );
    m_svgBack->setZValue( -30 );
    m_svgBack->setTransform( QTransform::fromScale( 1, 1), true );
    m_svgBack->setTransformOriginPoint( 120,120 );
    m_scene->addItem( m_svgBack );

    m_svgFace = new QGraphicsSvgItem( "/tmp/images/ai/ai_face.svg" );
    m_svgFace->setCacheMode( QGraphicsItem::NoCache );
    m_svgFace->setZValue( -20 );
    m_svgFace->setTransform( QTransform::fromScale( 1,1 ), true );
    m_svgFace->setTransformOriginPoint(  120,120 );
    m_scene->addItem( m_svgFace );

    m_svgRing = new QGraphicsSvgItem( "/tmp/images/ai/ai_ring.svg" );
    m_svgRing->setCacheMode( QGraphicsItem::NoCache );
    m_svgRing->setZValue( -10 );
    m_svgRing->setTransform( QTransform::fromScale( 1,1 ), true );
    m_svgRing->setTransformOriginPoint(  120,120 );
    m_scene->addItem( m_svgRing );

    m_svgCase = new QGraphicsSvgItem( "/tmp/images/ai/ai_case.svg" );
    m_svgCase->setCacheMode( QGraphicsItem::NoCache );
    m_svgCase->setZValue( 10 );
    m_svgCase->setTransform( QTransform::fromScale( 1,1 ), true );
    m_scene->addItem( m_svgCase );

    setScene(m_scene);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  }

  virtual ~AttitudeIndicator() {
    if (m_scene != Q_NULLPTR) {
      m_scene->clear();
      delete m_scene;
      m_scene = Q_NULLPTR;
    }

  }

  void setAttitude(double roll, double pitch) {
    double rollRad = M_PI * roll / 180.0;

    if (pitch > 30) {
      pitch = 30;
    } else if (pitch < -30){
      pitch = -30;
    }
    double delta = pitch * 1.7;

    m_svgBack->setRotation(-roll);
    m_svgFace->setRotation(-roll);
    m_svgRing->setRotation(-roll);

    double newX = delta * sin(rollRad);
    double newY = delta * cos(rollRad);

    m_svgFace->moveBy(newX - m_oldXPosition, -1 * (newY - m_oldYPosition));
    m_oldXPosition = newX;
    m_oldYPosition = newY;
  }

private:
  QGraphicsScene *m_scene;
  QGraphicsSvgItem *m_svgBack;
  QGraphicsSvgItem *m_svgFace;
  QGraphicsSvgItem *m_svgRing;
  QGraphicsSvgItem *m_svgCase;

  double m_oldXPosition;
  double m_oldYPosition;
};

#endif // ATTITUDEINDICATOR_HPP
