#include "./lib/controllers/aileron.hpp"
#include "./lib/controllers/elevator.hpp"
#include "./lib/controllers/rudder.hpp"
#include "./lib/controllers/motor.hpp"
#include "./lib/controllers/test_switch.hpp"
#include "./lib/controllers/flight_switch.hpp"
#include "./lib/flag.hpp"

const uint8_t CONTROLLERS_COUNT = 4U;
const String PADDING = "000000000000000000000000";
uint8_t STATE = 0U;

mcu::lib::motor motor;
mcu::lib::aileron aileron;
mcu::lib::elevator elevator;
mcu::lib::rudder rudder;

mcu::lib::interface::controller *controllers[CONTROLLERS_COUNT];

mcu::lib::test_switch test_switch;
mcu::lib::flight_switch flight_switch;

void write_state() {
  uint32_t ms = static_cast<uint32_t>(millis());
  for (int i = 23; i >= 0; --i) {
    Serial.print(static_cast<bool>(bitRead(ms, i)));
  }
  for (int i = CONTROLLERS_COUNT - 1; i >= 0; --i) {
    controllers[i]->write_state();
  }
  for (int i = 7; i >= 0; --i) {
    Serial.print(static_cast<bool>(bitRead(STATE, i)));
  }
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  test_switch.setup();
  flight_switch.setup();

  controllers[0] = &motor;
  controllers[1] = &aileron;
  controllers[2] = &elevator;
  controllers[3] = &rudder;

  for (auto ctr : controllers) {
    ctr->setup();
  }

  delay(2000);

  uint8_t i = 0;
  write_state();
  for (auto ctr : controllers) {
    ctr->test();
    STATE |= mcu::lib::flag::TEST_FLAGS[i++];
    write_state();
  }
  delay(1000);
}

void loop() {
  if (flight_switch.state()) {
    STATE |= mcu::lib::flag::FLIGHT_MODE;
    for (auto ctr : controllers) {
      ctr->step();
    }
  } else {
    STATE &= ~mcu::lib::flag::FLIGHT_MODE;
    for (auto ctr : controllers) {
      ctr->stop();
    }
    delay(500);
  }
  write_state();
}
