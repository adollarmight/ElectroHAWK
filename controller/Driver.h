#ifndef _DRIVER_H_
#define _DRIVER_H_

#include <memory>
#include <utility>
#include "hardware.h"
#define X(p) p.first
#define Y(p) p.second

namespace Control {
  namespace Config {
    inline constexpr int frontLeftPin = 26;
    inline constexpr int frontRightPin = 25;
    inline constexpr int backLeftPin = 33;
    inline constexpr int backRightPin = 32;

    inline constexpr int voltagePin = 27;

    inline constexpr double MOTOR_IDLE_SPEED = 1100;
    inline constexpr double kP = 0.2;
    inline constexpr double kI = 0;
    inline constexpr double kD = 0;
  }

  class Driver {
  private:
    std::unique_ptr<Hardware::Motor> frontLeft;
    std::unique_ptr<Hardware::Motor> frontRight;
    std::unique_ptr<Hardware::Motor> backLeft;
    std::unique_ptr<Hardware::Motor> backRight;
    std::unique_ptr<Hardware::IMU> imu;
    std::unique_ptr<Hardware::Voltage> voltage;

    int xAxis, yAxis, zAxis, rotation;
    bool running = true;

    std::thread controllerThread;
    std::mutex controllerMutex;

    double desiredRoll = 0, desiredYaw = 0, desiredPitch= 0;
    double power = 0;
    double lastRollError = 0, rollErrorSum = 0;
    double lastPitchError = 0, pitchErrorSum = 0;

    void setMotors(double fl, double fr, double bl, double br) {
      this->frontRight->setSpeed(fr);
      this->frontLeft->setSpeed(fl);
      this->backRight->setSpeed(br);
      this->backLeft->setSpeed(bl);
    }

    void controllerHandle() {
      while (running) {
        double rollError = desiredRoll - imu->getKalmanRollAngle();
        double pitchError = desiredPitch - (-imu->getKalmanPitchAngle());

        // rate of change in error (Newton is proud frfr)
        double rollDerivative = rollError - lastRollError;
        double pitchDerivative = pitchError - lastPitchError;

        rollErrorSum += rollError;
        pitchErrorSum += pitchError;

        double rollPower = Config::kP * rollError + Config::kI * rollErrorSum + Config::kD * rollDerivative;
        double pitchPower = Config::kP * pitchError + Config::kI * pitchError + Config::kD * pitchDerivative;
        double frontLeftMultiplier = rollPower + pitchPower;
        double frontRightMultiplier = -rollPower + pitchPower;
        double backLeftMultiplier = -rollPower - pitchPower;
        double backRightMultiplier = rollPower - pitchPower;
        setMotors(
          power * frontLeftMultiplier, 
          power * frontRightMultiplier, 
          power * backLeftMultiplier, 
          power * backRightMultiplier
        );

        lastRollError = rollError;
        lastPitchError = pitchError;
      }
    }

    double mapf(double x, double inMin, double inMax, double outMin, double outMax) {
      return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
    }

  public:
    Driver(
      int frontLeftPin = Config::frontLeftPin,
      int frontRightPin = Config::frontRightPin,
      int backLeftPin = Config::backLeftPin,
      int backRightPin = Config::backRightPin,
      int voltagePin = Config::voltagePin
    ): 
      frontLeft(new Hardware::Motor(frontLeftPin)),
      frontRight(new Hardware::Motor(frontRightPin)),
      backLeft(new Hardware::Motor(backLeftPin)),
      backRight(new Hardware::Motor(backRightPin)),
      imu(new Hardware::IMU()),
      voltage(new Hardware::Voltage(voltagePin))
    {
      delay(5000);
      setMotors(0.1, 0.1, 0.1, 0.1);
      delay(5000);
      setMotors(0, 0, 0, 0);
      controllerThread = std::thread(&Driver::controllerHandle, this);
    }

    ~Driver() {
      controllerMutex.lock();
      running = false;
      controllerMutex.unlock();
      controllerThread.join();
    }

    void setControls(data::ControlData controlData) {
      this->xAxis = controlData.xAxis;
      this->yAxis = controlData.yAxis;
      this->zAxis = controlData.zAxis;
      this->rotation = controlData.rotation;
      lastRollError = 0, rollErrorSum = 0;
      lastPitchError = 0, pitchErrorSum = 0;
      power = mapf(this->zAxis, -10.0, 10.0, 0.0, 1.0);
      desiredPitch = mapf(this->xAxis, -10.0, 10.0, 0.0, 1.0); 
    }

    data::SensorData getSensorData() {
      data::SensorData sensorData;
      sensorData.pitchAngle = imu->getKalmanPitchAngle();
      sensorData.rollAngle = imu->getKalmanRollAngle();
      sensorData.voltage = voltage->getVoltage();
      // sensorData.temperature = altimeter->getTemperature();
      // sensorData.pressure = altimeter->getPressure();
      // sensorData.altitude = altimeter->getAltitude();
      sensorData.temperature = 0;
      sensorData.pressure = 0;
      sensorData.altitude = 0;
      return sensorData;
    }
  };
}

#endif