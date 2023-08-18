#include <Arduino.h>
#include <packet.hpp>
#include <MPU6050_tockn.h>
#include <Wire.h>

const uint8_t kMOTOR_IN = 5U;
const uint8_t kAILERON_IN = 2U;
const uint8_t kELEVATOR_IN = 3U;
const uint8_t kRUDDER_IN = 4U;
const uint8_t kASSITANCE_IN = 6U;
const uint8_t kAUTOPILOT_IN = 7U;
const uint8_t kELEVATOR_OUT = 8U;

const uint16_t kASSISTANCE_FLAG = 0x10;
const uint16_t kUSER_INPUT = 0x20;
const uint16_t kTEST_FLAGS[] = {0x1, 0x4, 0x4, 0x08};

const double PVM_MIN = 1000.00;
const double PVM_MAX = 2000.00;

double toRange(double value, double MIN_OFFSET, double MAX_OFFSET) {
  return ((value - PVM_MIN) / (PVM_MAX - PVM_MIN)) * (MAX_OFFSET - MIN_OFFSET)
      + MIN_OFFSET;
}

rcplane::common::control_surface_packet cs_packet;
rcplane::common::orientation_packet ori_packet;

MPU6050 mpu(Wire);

void setup() {
  Serial.begin(115200);

  /**
   * Flush the serial buffer and await for a byte to be transmitted before
   * we continue. This is to avoid the som incorrectly parsing old data on the serial 
   * buffer and breaking preconditions.
   */
  Serial.flush();
  Serial.println("to be flushed");
  Serial.println("rcplane");
  while (Serial.available() == 0);
  Serial.read();
  Serial.flush();

  cs_packet.state = 0U;

  /**
   * Setup required pins.
   */
  pinMode(kMOTOR_IN, INPUT);
  pinMode(kAILERON_IN, INPUT);
  pinMode(kELEVATOR_IN, INPUT);
  pinMode(kRUDDER_IN, INPUT);
  pinMode(kASSITANCE_IN, INPUT);

  pinMode(kELEVATOR_OUT, OUTPUT);

  Wire.begin();
  mpu.begin();
  mpu.calcGyroOffsets();

  /**
   * TODO: Actually run tests. 
   */
  for (int i = 0; i < 4; ++i) {
    cs_packet.state |= kTEST_FLAGS[i];
  }
}

void loop() {
  /**
   * Populate the control surface packet and wrtie to the som.
   */
  cs_packet.state = 0x0F; // Test all done
  cs_packet.timestamp = millis();
  cs_packet.motor = toRange(pulseIn(kMOTOR_IN, HIGH), 0, 255);
  cs_packet.aileron = static_cast<uint8_t>(toRange(pulseIn(kAILERON_IN, HIGH), -30, 30));
  cs_packet.elevator = static_cast<uint8_t>(toRange(pulseIn(kELEVATOR_IN, HIGH), -50, 50));
  cs_packet.rudder = static_cast<uint8_t>(toRange(pulseIn(kRUDDER_IN, HIGH), -30, 30));

  cs_packet.state |= pulseIn(kASSITANCE_IN, HIGH) > 1600 ? kASSISTANCE_FLAG : 0x0;

  rcplane::common::write_packet<rcplane::common::control_surface_packet>(cs_packet);

  mpu.update();
  ori_packet.roll = mpu.getAngleX();
  ori_packet.pitch = mpu.getAngleY();
  ori_packet.yaw = mpu.getAngleZ();

  rcplane::common::write_packet<rcplane::common::orientation_packet>(ori_packet);

  cs_packet.timestamp = rcplane::common::read_packet<rcplane::common::control_surface_packet>(cs_packet);

  // Simulate writing to servos
  analogWrite(kELEVATOR_OUT, cs_packet.elevator + 115);
  analogWrite(kELEVATOR_OUT, cs_packet.elevator + 115);
  analogWrite(kELEVATOR_OUT, cs_packet.elevator + 115);
  analogWrite(kELEVATOR_OUT, cs_packet.elevator + 115);
}
