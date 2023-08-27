#include <Arduino.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

#include <State.hpp>
#include <Packet.hpp>

const uint8_t kMOTOR_IN = 5U;
const uint8_t kAILERON_IN = 2U;
const uint8_t kELEVATOR_IN = 3U;
const uint8_t kRUDDER_IN = 4U;
const uint8_t kASSISTANCE_IN = 6U;
const uint8_t kAUTOPILOT_IN = 7U;
const uint8_t kELEVATOR_OUT = 8U;

const double PVM_MIN = 1000.00;
const double PVM_MAX = 2000.00;

inline double toRange(double value, double MIN_OFFSET, double MAX_OFFSET) {
  return ((value - PVM_MIN) / (PVM_MAX - PVM_MIN)) * (MAX_OFFSET - MIN_OFFSET)
      + MIN_OFFSET;
}

bool has_user_input(int8_t &value) {
  return value >= 3 || value <= -3;
}

rcplane::common::HandshakePacket handshakePacket;
rcplane::common::StatePacket statePacket;
rcplane::common::ControlSurfacePacket csPacket;
rcplane::common::ImuPacket imuPacket;

MPU6050 mpu(Wire);

void setup() {
  Serial.begin(115200);

  // Flush the serial buffer and await for a byte to be transmitted before
  // we continue. This is to avoid the som incorrectly parsing old data on the serial 
  //buffer and breaking preconditions. 
  Serial.flush();
  Serial.println("flush");
  rcplane::common::readPacket<rcplane::common::HandshakePacket>(handshakePacket);
  rcplane::common::writePacket<rcplane::common::HandshakePacket>(handshakePacket);
  
  pinMode(kMOTOR_IN, INPUT);
  pinMode(kAILERON_IN, INPUT);
  pinMode(kELEVATOR_IN, INPUT);
  pinMode(kRUDDER_IN, INPUT);
  pinMode(kASSISTANCE_IN, INPUT);

  pinMode(kELEVATOR_OUT, OUTPUT);

  Wire.begin();
  mpu.begin();
  mpu.calcGyroOffsets();
}

void loop() {
  /**
   * Populate the control surface packet and write to the som.
   */
  csPacket.motorSpeed = toRange(pulseIn(kMOTOR_IN, HIGH), 0, 255);
  csPacket.aileronDeflection = static_cast<uint8_t>(toRange(pulseIn(kAILERON_IN, HIGH), -100, 100));
  csPacket.elevatorDeflection = static_cast<uint8_t>(toRange(pulseIn(kELEVATOR_IN, HIGH), -100, 100));
  csPacket.rudderDeflection = static_cast<uint8_t>(toRange(pulseIn(kRUDDER_IN, HIGH), -100, 100));


  statePacket.timestamp = millis();
  statePacket.state = 0x0;
  statePacket.state |= pulseIn(kASSISTANCE_IN, HIGH) > 1600 ? rcplane::common::state::kASSISTANCE_FLAG : 0x0;
  statePacket.state |= has_user_input(csPacket.aileronDeflection) ? rcplane::common::state::kUSER_ROLL : 0x0;
  statePacket.state |= has_user_input(csPacket.elevatorDeflection) ? rcplane::common::state::kUSER_PITCH : 0x0;

  rcplane::common::writePacket<rcplane::common::StatePacket>(statePacket);
  rcplane::common::writePacket<rcplane::common::ControlSurfacePacket>(csPacket);

  mpu.update();
  imuPacket.gyroX = mpu.getAngleX();
  imuPacket.gyroY = mpu.getAngleY();
  imuPacket.gyroZ = mpu.getAngleZ();
  imuPacket.accX = mpu.getAccX();
  imuPacket.accY = mpu.getAccY();
  imuPacket.accZ = mpu.getAccZ();
  imuPacket.temperature = mpu.getTemp();

  rcplane::common::writePacket<rcplane::common::ImuPacket>(imuPacket);

  rcplane::common::readPacket<rcplane::common::ControlSurfacePacket>(csPacket);

  // Simulate writing to servos
  analogWrite(kELEVATOR_OUT, csPacket.elevatorDeflection + 115);
  analogWrite(kELEVATOR_OUT, csPacket.elevatorDeflection + 115);
  analogWrite(kELEVATOR_OUT, csPacket.elevatorDeflection + 115);
  analogWrite(kELEVATOR_OUT, csPacket.elevatorDeflection + 115);
}