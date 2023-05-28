#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <string>
#include "Hardware.h"
// #include "Communication.h"

namespace Config {
  inline constexpr int RX = 16;
  inline constexpr int TX = 17;
  inline constexpr int RTS = 12;
  inline constexpr int CTS = 14;
}

class Instance {
private:
  // Communication::UART<
  //   Communication::SensorData, 
  //   Communication::DirectionData
  //   > communication;

  Hardware::IMU* imu;
  Hardware::Motor frontLeft, frontRight, backLeft, backRight;

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
  }

  void update() {
    // communication.send({ 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0 });
    // if (communication.receivedDataCount()) {
    //   Communication::DirectionData direction = communication.recieve();
    //   Serial.print(direction.direction);
    //   Serial.print(" ");
    //   Serial.println(direction.magnitude);
    // }

    // Serial.print(imu->getRoll());
    // Serial.print(" ");
    // Serial.print(imu->getPitch());
    // Serial.print(" ");
    // Serial.println(imu->getYaw());
    // Serial.print(imu->getRollAngle());
    // Serial.print(" ");
    // Serial.println(imu->getPitchAngle());
    Serial.print(imu->getKalmanRollAngle());
    Serial.print(" ");
    Serial.println(imu->getKalmanPitchAngle());
    Serial.println();

    delay(100);
  }
};

#endif