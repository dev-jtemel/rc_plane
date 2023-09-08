#ifndef HEADINGINDICATOR_HPP
#define HEADINGINDICATOR_HPP

#include <QGraphicsSvgItem>
#include <QGraphicsView>

class HeadingIndicator : public QGraphicsView {
  Q_OBJECT

public:
  explicit HeadingIndicator(QWidget *parent = Q_NULLPTR) : QGraphicsView(parent) {
    m_scene = new QGraphicsScene(this);
    m_svgFace = new QGraphicsSvgItem( "/tmp/images/hi/hi_face.svg" );
    m_svgFace->setCacheMode( QGraphicsItem::NoCache );
    m_svgFace->setZValue( -20 );
    m_svgFace->setTransform( QTransform::fromScale( 1,1 ), true );
    m_svgFace->setTransformOriginPoint(  120,120 );
    m_scene->addItem( m_svgFace );

    m_svgCase = new QGraphicsSvgItem( "/tmp/images/hi/hi_case.svg" );
    m_svgCase->setCacheMode( QGraphicsItem::NoCache );
    m_svgCase->setZValue( 10 );
    m_svgCase->setTransform( QTransform::fromScale( 1,1 ), true );
    m_scene->addItem( m_svgCase );

    setScene(m_scene);
  }

  virtual ~HeadingIndicator() {
    if (m_scene != Q_NULLPTR) {
      m_scene->clear();
      delete m_scene;
      m_scene = Q_NULLPTR;
    }

  }

  void setHeading(double heading) {
    m_svgFace->setRotation(heading);
  }

private:
  QGraphicsScene *m_scene;
  QGraphicsSvgItem *m_svgFace;
  QGraphicsSvgItem *m_svgCase;
};

#endif // HEADINGINDICATOR_HPP
