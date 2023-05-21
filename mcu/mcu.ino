#include "lib/aileron.hpp"
#include "lib/rudder.hpp"

mcu::lib::aileron aileron;
mcu::lib::rudder rudder;

void setup() {
  Serial.begin(115200);
  aileron.setup();  
  rudder.setup();  

  aileron.test();
  rudder.test();
}

void loop() {
  aileron.step();
  rudder.step();
}
