#include "./lib/controllers/aileron.hpp"
#include "./lib/controllers/elevator.hpp"
#include "./lib/controllers/rudder.hpp"
#include "./lib/controllers/motor.hpp"
#include "./lib/controllers/test_switch.hpp"
#include "./lib/controllers/flight_switch.hpp"
#include "./lib/controllers/imu.hpp"
#include "./lib/flag.hpp"

const uint8_t CONTROLLERS_COUNT = 4U;
const String PADDING = "000000000000000000000000";
const String START = "FFFFFFFFFFFFFFFF";
const String IMU_BLANK = "0000000000000000";
uint8_t STATE = 0U;

mcu::lib::motor motor;
mcu::lib::aileron aileron;
mcu::lib::elevator elevator;
mcu::lib::rudder rudder;

mcu::lib::imu imu;

mcu::lib::interface::controller *controllers[CONTROLLERS_COUNT];

mcu::lib::test_switch test_switch;
mcu::lib::flight_switch flight_switch;

void write_state() {
  uint32_t ms = static_cast<uint32_t>(millis());
  for (int i = 5; i >= 0; --i) {
    Serial.print(0xF & (ms >> i * 4), HEX);
  }
  for (int i = CONTROLLERS_COUNT - 1; i >= 0; --i) {
    controllers[i]->write_state();
  }
  Serial.print(STATE >> 4, HEX);
  Serial.print((0x0F & STATE), HEX);
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  delay(2000);

/*
  Serial.println(START);
  Serial.println(START);
  Serial.println(START);
  Serial.println(START);

  flight_switch.setup();

  controllers[0] = &motor;
  controllers[1] = &aileron;
  controllers[2] = &elevator;
  controllers[3] = &rudder;

  for (auto ctr : controllers) {
    ctr->setup();
  }

  imu.setup();

  delay(2000);

  uint8_t i = 0;
  write_state();
  Serial.println(IMU_BLANK);
  Serial.println(IMU_BLANK);
  Serial.println(IMU_BLANK);

  for (auto ctr : controllers) {
    ctr->test();
    STATE |= mcu::lib::flag::TEST_FLAGS[i++];
    write_state();
    Serial.println(IMU_BLANK);
    Serial.println(IMU_BLANK);
    Serial.println(IMU_BLANK);
  }
  delay(1000);
*/

  Serial.read();
  Serial.println(START);
  Serial.println(START);
  Serial.println(START);
  Serial.println(START);
  STATE |= mcu::lib::flag::FLIGHT_MODE;
}

void loop() {
  delay(200);
/*
  for (auto ctr : controllers) {
    ctr->step();
  }
  write_state();
  imu.step();
*/

  Serial.println(IMU_BLANK);
  Serial.println(IMU_BLANK);
  Serial.println(IMU_BLANK);
  Serial.println(IMU_BLANK);
}
