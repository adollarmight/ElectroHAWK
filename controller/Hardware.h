#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include <thread>
#include <mutex>
#include <utility>
#include <ESP32_Servo.h>
#include "Wire.h"

namespace Hardware {
  namespace Config {
    namespace Motor {
      inline constexpr std::pair<int, int> pulseWidth = {1000, 2000};
    }

    namespace IMU {
      inline constexpr int CalibrationCount = 4000;
      inline constexpr double AccelerationXOffset = -0.07, AccelerationYOffset = -0.04, AccelerationZOffset = -0.075;
      inline constexpr int ReadDataMicrosecondsDelay = 2500;
    }
  }

  inline void initialize() {
    // #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    //   Wire.begin();
    //   Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    // #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    //   Fastwire::setup(400, true);
    // #endif
  }

  class Motor {
    private:
      Servo controller;
    
    public:
      Motor(int pin) {
        controller.attach(
          pin, 
          Config::Motor::pulseWidth.first, 
          Config::Motor::pulseWidth.second
        );
        delay(250);
        controller.write(0);

        // analogWriteFrequency(pin, 250);
        // analogWriteResolution(12);
        // delay(250);
        // analogWrite(pin, 0);
      }

      void setSpeed(double speed) {
        if (speed > 1)
          speed = 1;
        else if (speed < 0)
          speed = 0;
        controller.write(floor(speed * 180));
        // analogWrite(pin, 1.024 * speed);
      }
  };

  class IMU {
  private:
    bool running = true;

    std::thread readThread;
    std::mutex readMutex;

    double yawRate, pitchRate, rollRate;
    double yawOffset, pitchOffset, rollOffset;

    double accelerationX, accelerationY, accelerationZ;

    double rollAngle, pitchAngle;

    double kalmanRollAngle = 0, kalmanUncertaintyRollAngle = 2 * 2;
    double kalmanPitchAngle = 0, kalmanUncertaintyPitchAngle = 2 * 2;
    double kalman1DOutput[2] = {0, 0};

    void kalman_1d(double state, double uncertainty, double input, double measurement) {
      state = state + 0.004 * input;
      uncertainty = uncertainty + 0.004 * 0.004 * 4 * 4;
      double kalmanGain=uncertainty * 1 / (1 * uncertainty + 3 * 3);
      state = state + kalmanGain * (measurement - state);
      uncertainty = (1 - kalmanGain) * uncertainty;
      kalman1DOutput[0] = state; 
      kalman1DOutput[1] = uncertainty;
    }

    void readHandle() {
      while (running) {
        readData();

        rollRate -= rollOffset;
        pitchRate -= pitchOffset;
        yawRate -= yawOffset;

        // int rollAux = (int)(rollRate * 1000) - (int)(rollOffset * 1000);
        // int pitchAux = (int)(pitchRate * 1000) - (int)(pitchOffset * 1000);
        // int yawAux = (int)(yawRate * 1000) - (int)(yawOffset * 1000);

        // rollRate = (double)rollAux / 1000;
        // pitchRate = (double)pitchAux / 1000;
        // yawRate = (double)yawAux / 1000;

        kalman_1d(kalmanRollAngle, kalmanUncertaintyRollAngle, rollRate, rollAngle);
        kalmanRollAngle=kalman1DOutput[0]; 
        kalmanUncertaintyRollAngle=kalman1DOutput[1];

        kalman_1d(kalmanPitchAngle, kalmanUncertaintyPitchAngle, pitchRate, pitchAngle);
        kalmanPitchAngle=kalman1DOutput[0]; 
        kalmanUncertaintyPitchAngle=kalman1DOutput[1];

        delayMicroseconds(Config::IMU::ReadDataMicrosecondsDelay);
      }
    }

    void readData() {
      Wire.beginTransmission(0x68);
      Wire.write(0x1A);
      Wire.write(0x05);
      Wire.endTransmission();
      Wire.beginTransmission(0x68);
      Wire.write(0x1C);
      Wire.write(0x10);
      Wire.endTransmission();
      Wire.beginTransmission(0x68);
      Wire.write(0x3B);
      Wire.endTransmission(); 
      Wire.requestFrom(0x68,6);
      int16_t AccXLSB = Wire.read() << 8 | Wire.read();
      int16_t AccYLSB = Wire.read() << 8 | Wire.read();
      int16_t AccZLSB = Wire.read() << 8 | Wire.read();

      Wire.beginTransmission(0x68);
      Wire.write(0x1B); 
      Wire.write(0x8);
      Wire.endTransmission();     
      Wire.beginTransmission(0x68);
      Wire.write(0x43);
      Wire.endTransmission();
      Wire.requestFrom(0x68,6);
      int16_t GyroX = Wire.read()<<8 | Wire.read();
      int16_t GyroY = Wire.read()<<8 | Wire.read();
      int16_t GyroZ = Wire.read()<<8 | Wire.read();

      rollRate = (double)GyroX / 65.5;
      pitchRate = (double)GyroY / 65.5;
      yawRate = (double)GyroZ / 65.5;

      accelerationX = (double)AccXLSB / 4096 + Config::IMU::AccelerationXOffset;
      accelerationY = (double)AccYLSB / 4096 + Config::IMU::AccelerationYOffset;
      accelerationZ = (double)AccZLSB / 4096 + Config::IMU::AccelerationZOffset;

      rollAngle = atan(accelerationY / sqrt(accelerationX * accelerationX + accelerationZ * accelerationZ)) * 1 / (3.142 / 180);
      pitchAngle = -atan(accelerationX / sqrt(accelerationY * accelerationY + accelerationZ * accelerationZ)) * 1 / (3.142 / 180);
    }

  public:
    IMU() {
      Wire.setClock(400000);
      Wire.begin();
      delay(250);
      Wire.beginTransmission(0x68); 
      Wire.write(0x6B);
      Wire.write(0x00);
      Wire.endTransmission();

      int yawAux;
      int pitchAux;
      int rollAux;

      for (int i = 0; i < Config::IMU::CalibrationCount; i++) {
        readData();
        yawAux += yawRate * 1000;
        pitchAux += pitchRate * 1000;
        rollAux += rollRate * 1000;
        delay(1);
      }

      yawOffset = (double)yawAux / 1000 / Config::IMU::CalibrationCount;
      pitchOffset = (double)pitchAux / 1000 / Config::IMU::CalibrationCount;
      rollOffset = (double)rollAux / 1000 / Config::IMU::CalibrationCount;

      readThread = std::thread(&IMU::readHandle, this);
    }

    ~IMU() {
      readMutex.lock();
      running = false;
      readMutex.unlock();
      readThread.join();
    }

    double getYaw() const { return yawRate; }
    double getPitch() const { return pitchRate; }
    double getRoll() const { return rollRate; }
    double getRollAngle() const { return rollAngle; }
    double getPitchAngle() const { return pitchAngle; }
    double getKalmanRollAngle() const { return kalmanRollAngle; }
    double getKalmanPitchAngle() const { return kalmanPitchAngle; }
  };

  class Voltage {
  private:
    bool running = true;

    std::thread readThread;
    std::mutex readMutex;

    int pin;

    double voltage;

    void readHandle() {
      while (running) {
        voltage = analogRead(pin) * (3.3 / 1023.0);

        // voltage = (double)analogRead(15) / 62;
        // current = (double)analogRead(21) * 0.089;

        delay(50);
      }
    }

  public:
    Voltage(int pin) {
      this->pin = pin;
    }

    ~Voltage() {
      readMutex.lock();
      running = false;
      readMutex.unlock();
      readThread.join();
    }

    double getVoltage() { return voltage; }
  };
}

#endif