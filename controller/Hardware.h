#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include <thread>
#include <mutex>
#include <utility>
#include <ESP32_Servo.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

namespace Hardware {
  namespace Config {
    namespace Motor {
      inline constexpr std::pair<int, int> pulseWidth = {1000, 2000};
    }
    namespace Gyro {
      inline constexpr int XGyroOffset = 0;
      inline constexpr int YGyroOffset = 0;
      inline constexpr int ZGyroOffset = 0;
      inline constexpr int ZAccelOffset = 1000;
    }
  }

  // initialize everything hardware oriented
  inline void initialize() {
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
      Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
    #endif
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
        controller.write(0);
      }

      void setSpeed(double speed) {
        if (speed > 1)
          speed = 1;
        else if (speed < 0)
          speed = 0;
        controller.write(floor(speed * 180));
      }
  };

  class IMU {
  private:
    MPU6050 mpu;
    uint16_t packetSize;
    bool running = true;

    double yaw = 69, pitch = 69, roll = 69;
    std::thread readThread;
    std::mutex readMutex;
    void readHandle() {
      uint8_t mpuIntStatus;
      uint16_t fifoCount;
      uint8_t fifoBuffer[64];
      Quaternion q;
      VectorFloat gravity;
      float ypr[3];

      while (running) {
        mpu.dmpGetCurrentFIFOPacket(fifoBuffer);
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

        readMutex.lock();
        yaw = ypr[0];
        pitch = ypr[1];
        roll = ypr[2];
        readMutex.unlock();
      }
    }

  public:
    IMU() {
      mpu.initialize();
      uint8_t status = mpu.dmpInitialize();
      mpu.setXGyroOffset(Config::Gyro::XGyroOffset);
      mpu.setYGyroOffset(Config::Gyro::YGyroOffset);
      mpu.setZGyroOffset(Config::Gyro::ZGyroOffset);
      mpu.setZAccelOffset(Config::Gyro::ZAccelOffset);
      if (status != 0) {
        Serial.println("a dat lumea de necaz");
        while (1) delay(500);
      }

      mpu.CalibrateAccel();
      mpu.CalibrateGyro();
      mpu.setDMPEnabled(true);
      mpu.getIntStatus();
      packetSize = mpu.dmpGetFIFOPacketSize();

      readThread = std::thread(&IMU::readHandle, this);
    }

    ~IMU() {
      readMutex.lock();
      running = false;
      readMutex.unlock();
      readThread.join();
    }

    double getYaw() const { return yaw; }
    double getPitch() const { return pitch; }
    double getRoll() const { return roll; }
  };
}

#endif