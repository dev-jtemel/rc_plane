#include <Arduino.h>

const String IMU_BLANK = "0000000000000000";
const int BUFFER_SIZE = 4;
char buf[BUFFER_SIZE];

const uint8_t MOTOR_IN = 5U;
const uint8_t AILERON_IN = 2U;
const uint8_t ELEVATOR_IN = 3U;
const uint8_t RUDDER_IN = 4U;

const uint16_t FLIGHT_MODE = 0x1;
const uint16_t TEST_FLAGS[] = {0x2, 0x4, 0x8, 0x10, 0x0, 0x0};

uint8_t STATE = 0U;

const uint8_t CONTROLLERS_COUNT = 4U;
uint8_t controllers[CONTROLLERS_COUNT]; //motor, aileron, elevator, rudder

const double PVM_MIN = 1000.00;
const double PVM_MAX = 2000.00;

double toRange(double value, double MIN_OFFSET, double MAX_OFFSET) {
  return ((value - PVM_MIN) / (PVM_MAX - PVM_MIN)) * (MAX_OFFSET - MIN_OFFSET)
      + MIN_OFFSET;
}

void write_state() {
  uint32_t ms = static_cast<uint32_t>(millis());
  for (int i = 5; i >= 0; --i) {
    Serial.print(0xF & (ms >> i * 4), HEX);
  }
  for (int i = CONTROLLERS_COUNT - 1; i >= 0; --i) {
    Serial.print(controllers[i] >> 4, HEX);
    Serial.print((0x0F & controllers[i]), HEX);
  }

  Serial.print(STATE >> 4, HEX);
  Serial.print((0x0F & STATE), HEX);
  Serial.println();
}

void setup() {
  Serial.begin(115200);

  Serial.flush();
  Serial.println("rcplane");
  String s = "";
  while (s != "rcplane") {
    s = Serial.readString();
    s.trim();
  }

  pinMode(MOTOR_IN, INPUT);
  pinMode(AILERON_IN, INPUT);
  pinMode(ELEVATOR_IN, INPUT);
  pinMode(RUDDER_IN, INPUT);

  for (int i = 0; i < 4; ++i) {
    STATE |= TEST_FLAGS[i];
  }
  STATE |= FLIGHT_MODE;
}

void loop() {
  controllers[0] = toRange(pulseIn(MOTOR_IN, HIGH), 0, 255);
  controllers[1] = static_cast<uint8_t>(toRange(pulseIn(AILERON_IN, HIGH), -30, 30));
  controllers[2] = static_cast<uint8_t>(toRange(pulseIn(ELEVATOR_IN, HIGH), -50, 50));
  controllers[3] = static_cast<uint8_t>(toRange(pulseIn(RUDDER_IN, HIGH), -30, 30));
  
  write_state();
  Serial.println(IMU_BLANK);
  Serial.println(IMU_BLANK);
  Serial.println(IMU_BLANK);

  int rlen = Serial.readBytes(buf, BUFFER_SIZE);
  if (rlen != BUFFER_SIZE) {
    Serial.println("ERROR");
  }

}
