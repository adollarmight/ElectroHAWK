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

    inline constexpr double MotorIdleSpeed = 1100;
    inline constexpr double MotorCutoffSpeed = 0;
  }

  class Driver {
    std::unique_ptr<Hardware::Motor> frontLeft;
    std::unique_ptr<Hardware::Motor> frontRight;
    std::unique_ptr<Hardware::Motor> backLeft;
    std::unique_ptr<Hardware::Motor> backRight;
    std::unique_ptr<Hardware::IMU> imu;
    std::unique_ptr<Hardware::Voltage> voltage;

    int xAxis, yAxis, zAxis, rotation;

    double Kp = 0.5;
    double Ki = 0.001;
    double Kd = 0;

    double desiredPitch = 0.0;
    double desiredRoll = 0.0;

    double lastPitchError = 0.0;
    double lastRollError = 0.0;
    double totalPitchError = 0.0;
    double totalRollError = 0.0;

    double throttle = 1000;

    bool running = true;

    std::thread controllerThread;
    std::mutex controllerMutex;

    void controlMotors(double throttle, double pitchPID, double rollPID) {
      double frontRightSpeed = throttle + pitchPID;
      double frontLeftSpeed = throttle + rollPID;
      double backLeftSpeed = throttle - pitchPID;
      double backRightSpeed = throttle - rollPID;

      // Serial.print("Raw front right ");
      // Serial.println(frontRightSpeed);
      // Serial.print("Raw front left ");
      // Serial.println(frontLeftSpeed);
      // Serial.print("Raw back right ");
      // Serial.println(backRightSpeed);
      // Serial.print("Raw back left ");
      // Serial.println(backLeftSpeed);
      // Serial.println();
      // frontRight->setSpeed((frontRightSpeed - 1000) / 1000);
      // frontLeft->setSpeed((frontLeftSpeed - 1000) / 1000);
      // backRight->setSpeed((backRightSpeed - 1000) / 1000);
      // backLeft->setSpeed((backLeftSpeed - 1000) / 1000);
    }

    double calculatePIDOutput(double measuredValue, double desiredValue, double kp, double ki, double kd, double& lastError, double& totalError) {
      double error = desiredValue - measuredValue;
      totalError += error;
      double errorChange = error - lastError;

      double pidOutput = kp * error + ki * totalError + kd * errorChange;

      lastError = error;

      return pidOutput;
    }

    void controllerHandle() {
      while (running) {
        double pitchPID = calculatePIDOutput(imu->getKalmanPitchAngle(), desiredPitch, Kp, Ki, Kd, lastPitchError, totalPitchError);
        double rollPID = calculatePIDOutput(imu->getKalmanRollAngle(), desiredRoll, Kp, Ki, Kd, lastRollError, totalRollError);

        controlMotors(throttle, pitchPID, rollPID);

        delay(25);
      }
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
      Serial.println("Calibrating Motors");
      delay(5000);
      setMotors(0.1, 0.1, 0.1, 0.1);
      delay(5000);
      setMotors(0, 0, 0, 0);
      Serial.println("Motors Calibrated");

      controllerThread = std::thread(&Driver::controllerHandle, this);
    }

    ~Driver() {
      controllerMutex.lock();
      running = false;
      controllerMutex.unlock();
      controllerThread.join();
    }

    void stop() {
      controllerMutex.lock();

      running = false;

      controllerMutex.unlock();
    }

    void start() {
      controllerMutex.lock();

      running = true;

      controllerMutex.unlock();
    }

    void setMotors(double fr, double fl, double br, double bl) {
      this->frontRight->setSpeed(fr);
      this->frontLeft->setSpeed(fl);
      this->backRight->setSpeed(br);
      this->backLeft->setSpeed(bl);
    }

    void setControls(data::ControlData controlData) {
      this->xAxis = controlData.xAxis;
      this->yAxis = controlData.yAxis;
      this->zAxis = controlData.zAxis;
      this->rotation = controlData.rotation;

      // this->frontLeft->setSpeed((double)map(this->xAxis, -10, 10, 0, 100) / 100);
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