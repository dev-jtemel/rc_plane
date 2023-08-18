#include <Arduino.h>
#include <assert.h>
#include <packet.hpp>

const uint8_t MOTOR_IN = 5U;
const uint8_t AILERON_IN = 2U;
const uint8_t ELEVATOR_IN = 3U;
const uint8_t RUDDER_IN = 4U;

const uint16_t FLIGHT_MODE = 0x1;
const uint16_t TEST_FLAGS[] = {0x2, 0x4, 0x8, 0x10, 0x0, 0x0};

uint8_t STATE = 0U;

const double PVM_MIN = 1000.00;
const double PVM_MAX = 2000.00;

double toRange(double value, double MIN_OFFSET, double MAX_OFFSET) {
  return ((value - PVM_MIN) / (PVM_MAX - PVM_MIN)) * (MAX_OFFSET - MIN_OFFSET)
      + MIN_OFFSET;
}

rcplane::common::control_surface_packet cs_packet;

void setup() {
  Serial.begin(115200);

  Serial.flush();
  Serial.println("to be flushed");
  Serial.println("rcplane");
  while (Serial.available() == 0);
  Serial.read();

  pinMode(MOTOR_IN, INPUT);
  pinMode(AILERON_IN, INPUT);
  pinMode(ELEVATOR_IN, INPUT);
  pinMode(RUDDER_IN, INPUT);

  for (int i = 0; i < 4; ++i) {
    STATE |= TEST_FLAGS[i];
  }
  STATE |= FLIGHT_MODE;
  cs_packet.state = STATE;
}

void loop() {
  cs_packet.timestamp = millis();
  cs_packet.motor = toRange(pulseIn(MOTOR_IN, HIGH), 0, 255);
  cs_packet.aileron = static_cast<uint8_t>(toRange(pulseIn(AILERON_IN, HIGH), -30, 30));
  cs_packet.elevator = static_cast<uint8_t>(toRange(pulseIn(ELEVATOR_IN, HIGH), -50, 50));
  cs_packet.rudder = static_cast<uint8_t>(toRange(pulseIn(RUDDER_IN, HIGH), -30, 30));
  
  rcplane::common::write_packet<rcplane::common::control_surface_packet>(cs_packet);
}
