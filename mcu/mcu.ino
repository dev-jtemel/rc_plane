#include "lib/aileron.hpp"
#include "lib/elevator.hpp"
#include "lib/rudder.hpp"
#include "lib/logger.hpp"

mcu::lib::logger logger;
mcu::lib::aileron aileron;
mcu::lib::elevator elevator;
mcu::lib::rudder rudder;

mcu::lib::interface::controller *controllers[3];

void setup() {
  logger.init();
  delay(2000);

  controllers[0] = &aileron;
  controllers[1] = &elevator;
  controllers[2] = &rudder;

  for (auto ctr : controllers) {
    ctr->setup();
  }

  for (auto ctr : controllers) {
    ctr->test();
  }

  delay(2000);
}

void loop() {
  logger.log("******************");
  for (auto ctr : controllers) {
    ctr->step();
  }
}
