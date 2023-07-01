#ifndef __MCU__LIB__IMU_HPP__
#define __MCU__LIB__IMU_HPP__

#include "../interface/controller.hpp"
#include "../pins.hpp"
#include <Arduino.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

namespace mcu {
namespace lib {

typedef union {
  float value;
  byte binary[4];
} binary_float;

class imu : public interface::controller {
public:
  imu() : interface::controller(115, 30, -30), _mpu(Wire) {}

  ~imu() = default;

  virtual bool setup() {
    Wire.begin();

    _mpu.begin();
    _mpu.calcGyroOffsets();
  }

  virtual void test() {}

  virtual void step() {
    _mpu.update();
    _pitch.value = _mpu.getAngleX();
    _roll.value = _mpu.getAngleY();
    _yaw.value = _mpu.getAngleZ();

    _accx.value = _mpu.getAccX();
    _accy.value = _mpu.getAccY();
    _accz.value = _mpu.getAccZ();

    for (int i = 3; i >= 0; --i) {
      Serial.print(_pitch.binary[i] >> 4, HEX);
      Serial.print(0xF & _pitch.binary[i], HEX);
    }
    for (int i = 3; i >= 0; --i) {
      Serial.print(_roll.binary[i] >> 4, HEX);
      Serial.print(0xF & _roll.binary[i], HEX);
    }
    Serial.println();

    for (int i = 3; i >= 0; --i) {
      Serial.print(_yaw.binary[i] >> 4, HEX);
      Serial.print(0xF & _yaw.binary[i], HEX);
    }
    for (int i = 3; i >= 0; --i) {
      Serial.print(_accx.binary[i] >> 4, HEX);
      Serial.print(0xF & _accx.binary[i], HEX);
    }
    Serial.println();

    for (int i = 3; i >= 0; --i) {
      Serial.print(_accy.binary[i] >> 4, HEX);
      Serial.print(0xF & _accy.binary[i], HEX);
    }
    for (int i = 3; i >= 0; --i) {
      Serial.print(_accz.binary[i] >> 4, HEX);
      Serial.print(0xF & _accz.binary[i], HEX);
    }
    Serial.println();
  }

  virtual void stop() {}

private:
  MPU6050 _mpu;
  uint64_t timer = 0;
  float timeStep = 0.012;
  binary_float _pitch;
  binary_float _roll;
  binary_float _yaw;
  binary_float _accx;
  binary_float _accy;
  binary_float _accz;
};

}  // namespace lib
}  // namespace mcu

#endif  //__MCU__LIB__IMU_HPP__
