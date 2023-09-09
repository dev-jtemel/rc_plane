#ifndef CONTROLSTATION_HPP
#define CONTROLSTATION_HPP

#include <QMainWindow>
#include <QVariant>
#include <memory>

#include "HeadingIndicator.hpp"
#include "AttitudeIndicator.hpp"


QT_BEGIN_NAMESPACE
namespace Ui {
class ControlStation;
}
QT_END_NAMESPACE


class ControlStation : public QMainWindow {
  Q_OBJECT

public:
  ControlStation(QWidget *parent = nullptr);
  ~ControlStation();

public slots:
  void handleMessages(QVariant debug, QVariant attitude, QVariant onboard);

private:
  Ui::ControlStation *ui;

  AttitudeIndicator *m_attitudeIndicator;
  HeadingIndicator *m_headingIndicator;
};
#endif  // CONTROLSTATION_HPP
