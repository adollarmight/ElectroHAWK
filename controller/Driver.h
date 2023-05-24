#ifndef _DRIVER_H_
#define _DRIVER_H_

#include <memory>
#include "Hardware.h"

namespace Control {
  namespace Config {
    inline constexpr frontLeftPin = 25;
    inline constexpr frontRightPin = 26;
    inline constexpr backLeftPin = 32;
    inline constexpr backRightPin = 33;
  }

  // handles motor speed acording to the direction
  class Driver {
  private:
    Direction direction = Movement.Nothing;
    double magnitude = 0;

    // hardware
    std::unique_ptr<Hardware::Motor> frontLeft;
    std::unique_ptr<Hardware::Motor> frontRight;
    std::unique_ptr<Hardware::Motor> backLeft;
    std::unique_ptr<Hardware::Motor> backRight;
    std::unique_ptr<Hardware::IMU> imu;

    bool running = true;
    std::thread controllerThread;
    std::mutex controllerMutex;
    void controllerHandle() {
      while (running) {
        // todo: implement
        delay(500);
      }
    }

  public:
    Driver(
      int frontLeftPin = Config::frontLeftPin,
      int frontRightPin = Config::frontRightPin,
      int backLeftPin = Config::backLeftPin,
      int backRightPin = Config::backRightPin
    ) {
      frontLeft = std::make_unique<Hardware::Motor>(frontLeftPin);
      frontRight = std::make_unique<Hardware::Motor>(frontRightPin);
      backLeft = std::make_unique<Hardware::Motor>(backLeftPin);
      backRight = std::make_unique<Hardware::Motor>(backRightPin);
      imu = std::make_unique<Hardware::IMU>();

      controllerThread = std::thread(&Driver::controllerHandle, this);
    }

    ~Driver() {
      controllerMutex.lock();
      running = false;
      controllerMutex.unlock();
      controllerThread.join();
    }

    void updateDirection(Direction direction, double magnitude) {
      controllerMutex.lock();
      this->direction = direction;
      this->magnitude = magnitude;
      controllerMutex.unlock();
    }
  };
}

#endif