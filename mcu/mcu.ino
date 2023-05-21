#include "lib/aileron.hpp"
#include "lib/elevator.hpp"
#include "lib/rudder.hpp"

mcu::lib::aileron aileron;
mcu::lib::elevator elevator;
mcu::lib::rudder rudder;

void setup() {
  Serial.begin(115200);

  aileron.setup();  
  elevator.setup();
  rudder.setup();  

  aileron.test();
  elevator.test();
  rudder.test();
}

void loop() {
  aileron.step();
  elevator.step();
  rudder.step();
}
