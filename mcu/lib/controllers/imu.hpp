#ifndef __MCU__LIB__IMU_HPP__
#define __MCU__LIB__IMU_HPP__

#include <Wire.h>
#include <MPU6050.h>
#include <Arduino.h>
#include "../pins.hpp"
#include "../interface/controller.hpp"

namespace mcu {
namespace lib {

typedef union {
  float value;
  byte binary[4];
} binary_float;

class imu : public interface::controller {
 public:
  imu() : interface::controller(115, 30, -30) {
  }

  ~imu() = default;

  virtual bool setup() {
    while(!_mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
      delay(500);
    }
    _mpu.calibrateGyro();
    _mpu.setThreshold(3);
  }

  virtual void test() {
  }

  virtual void step() {
    Vector gyro = _mpu.readNormalizeGyro();
    _pitch.value += gyro.YAxis * timeStep;
    _roll.value += gyro.XAxis * timeStep;
    _yaw.value += gyro.ZAxis * timeStep;

    Vector acc = _mpu.readNormalizeAccel();
    _accx.value = acc.XAxis;
    _accy.value = acc.YAxis;
    _accz.value = acc.ZAxis;

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

  virtual void stop() {
  }

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

} // namespace lib
} // namespace mcu

#endif //__MCU__LIB__IMU_HPP__
