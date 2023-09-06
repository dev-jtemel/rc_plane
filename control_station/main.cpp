#include "ControlStation.hpp"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  ControlStation w;
  w.show();
  return a.exec();
}
