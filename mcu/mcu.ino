#include "lib/aileron.hpp"
#include "lib/elevator.hpp"
#include "lib/rudder.hpp"
#include "lib/motor.hpp"
#include "lib/logger.hpp"

mcu::lib::logger logger;

mcu::lib::motor motor;
mcu::lib::aileron aileron;
mcu::lib::elevator elevator;
mcu::lib::rudder rudder;

mcu::lib::interface::controller *controllers[4];

const uint8_t MOTOR_OUT_1 = 22;
const uint8_t MOTOR_OUT_2 = 24;
const uint8_t MOTOR_ENABLE = 12;

void setup() {
  logger.init();
  delay(2000);

  controllers[0] = &motor;
  controllers[1] = &aileron;
  controllers[2] = &elevator;
  controllers[3] = &rudder;

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
