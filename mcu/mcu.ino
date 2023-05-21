#include "lib/aileron.hpp"
#include "lib/elevator.hpp"
#include "lib/rudder.hpp"

mcu::lib::aileron aileron;
mcu::lib::elevator elevator;
mcu::lib::rudder rudder;

mcu::lib::interface::controller *controllers[3];

void setup() {
  Serial.begin(115200);

  controllers[0] = &aileron;
  controllers[1] = &elevator;
  controllers[2] = &rudder;

  for (auto ctr : controllers) {
    ctr->setup();
  }

  for (auto ctr : controllers) {
    ctr->test();
  }
}

void loop() {
  for (auto ctr : controllers) {
    ctr->step();
  }
}
