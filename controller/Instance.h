#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include "Hardware.h"

class Instance {
private:
  Hardware::Motor frontLeft, frontRight, backLeft, backRight;
  Hardware::IMU* imu;
public:
  Instance()
    : frontLeft(25), 
      frontRight(26), 
      backLeft(32), 
      backRight(33) 
  {
    Serial.begin(115200);
    Hardware::initialize();
    imu = new Hardware::IMU();
  }

  ~Instance() {
    delete imu;
  }

  void update() {
    Serial.print("Pitch: ");
    Serial.println(imu->getPitch() * 180 / M_PI);
    Serial.print("Yaw: "); 
    Serial.println(imu->getYaw() * 180 / M_PI);
    Serial.print("Roll: ");
    Serial.println(imu->getRoll() * 180 / M_PI);
    Serial.flush();
    delay(100);
  }
};

#endif