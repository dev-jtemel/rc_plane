#include "lib/aileron.hpp"

mcu::lib::aileron aileron;

void setup() {
  Serial.begin(115200);
  aileron.setup();  
  aileron.test();
}

void loop() {
  aileron.step();
}
