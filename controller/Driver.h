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

    double desiredRollRate, desiredPitchRate, desiredYawRate;
    double errorRollRate, errorPitchRate, errorYawRate;
    double inputRoll, inputThrottle, inputPitch, inputYaw;

    double lastErrorRollRate, lastErrorPitchRate, lastErrorYawRate;
    double lastKiTermRollRate, lastKiTermPitchRate, lastKiTermYawRate;

    double PIDReturn[3] = {0, 0, 0};

    double kpRollRate = 0.6; double kpPitchRate = kpRollRate; double kpYawRate = 2;
    double kiRollRate = 3.5; double kiPitchRate = kiRollRate; double kiYawRate = 12;
    double kdRollRate = 0.03; double kdPitchRate = kdRollRate; double kdYawRate = 0;

    double desiredRollAngle, desiredPitchAngle;
    double errorRollAngle, errorPitchAngle;
    double lastErrorRollAngle, lastErrorPitchAngle;
    double lastKiTermRollAngle, lastKiTermPitchAngle;

    double kpRollAngle = 2; double kpPitchAngle = kpRollAngle;
    double kiRollAngle = 0; double kiPitchAngle = kiRollAngle;
    double kdRollAngle = 0; double kdPitchAngle = kdRollAngle;

    // hardware
    std::unique_ptr<Hardware::Motor> frontLeft;
    std::unique_ptr<Hardware::Motor> frontRight;
    std::unique_ptr<Hardware::Motor> backLeft;
    std::unique_ptr<Hardware::Motor> backRight;
    std::unique_ptr<Hardware::IMU> imu;

    bool running = true;

    std::thread controllerThread;
    std::mutex controllerMutex;

    void CalulatePID(double error, double kp , double ki, double kd, double lastError, double lastKiTerm) {
      double kpTerm = kp * error;
      double kiTerm = lastKiTerm + ki * (error + lastError) * 0.004 / 2;

      if (kiTerm > 400) 
        kiTerm=400;
      else if (kiTerm < -400) 
        kiTerm=-400;

      double kdTerm = kd * (error - lastError) / 0.004;
      double PIDOutput = kpTerm + kiTerm + kdTerm;

      if (PIDOutput > 400) 
        PIDOutput = 400;
      else if (PIDOutput  -400) 
        PIDOutput = -400;

      PIDReturn[0]=PIDOutput;
      PIDReturn[1]=error;
      PIDReturn[2]=kiTerm;
    }

    void resetPID() {
      lastErrorRollRate=0; lastErrorPitchRate=0; lastErrorYawRate=0;
      lastKiTermRollRate=0; lastKiTermPitchRate=0; lastKiTermYawRate=0;
      lastErrorRollAngle=0; lastErrorPitchAngle=0;    
      lastKiTermRollAngle=0; lastKiTermPitchAngle=0;
    }

    void controllerHandle() {
      while (running) {
      //   desiredRollAngle=0.10*(ReceiverValue[0]-1500);
      //   desiredPitchAngle=0.10*(ReceiverValue[1]-1500);
      //   inputThrottle=ReceiverValue[2];
      //   desiredYawRate=0.15*(ReceiverValue[3]-1500);
      //   errorRollAngle=desiredRollAngle-KalmanAngleRoll;
      //   errorPitchAngle=desiredPitchAngle-KalmanAnglePitch;
      //   pid_equation(errorRollAngle, kpRollAngle, kiRollAngle, kdRollAngle, lastErrorRollAngle, lastKiTermRollAngle);     
      //   desiredRollRate=PIDReturn[0]; 
      //   lastErrorRollAngle=PIDReturn[1];
      //   lastKiTermRollAngle=PIDReturn[2];
      //   pid_equation(errorPitchAngle, kpPitchAngle, kiPitchAngle, kdPitchAngle, lastErrorPitchAngle, lastKiTermPitchAngle);
      //   desiredPitchRate=PIDReturn[0]; 
      //   lastErrorPitchAngle=PIDReturn[1];
      //   lastKiTermPitchAngle=PIDReturn[2];
      //   errorRollRate=desiredRollRate-RateRoll;
      //   errorPitchRate=desiredPitchRate-RatePitch;
      //   errorYawRate=desiredYawRate-RateYaw;
      //   pid_equation(errorRollRate, kpRollRate, kiRollRate, kdRollRate, lastErrorRollRate, lastKiTermRollRate);
      //       inputRoll=PIDReturn[0];
      //       lastErrorRollRate=PIDReturn[1]; 
      //       lastKiTermRollRate=PIDReturn[2];
      //   pid_equation(errorPitchRate, kpPitchRate,kiPitchRate, kdPitchRate, lastErrorPitchRate, lastKiTermPitchRate);
      //       inputPitch=PIDReturn[0]; 
      //       lastErrorPitchRate=PIDReturn[1]; 
      //       lastKiTermPitchRate=PIDReturn[2];
      //   pid_equation(errorYawRate, kpYawRate,kiYawRate, kdYawRate, lastErrorYawRate, lastKiTermYawRate);
      //       inputYaw=PIDReturn[0]; 
      //       lastErrorYawRate=PIDReturn[1]; 
      //       lastKiTermYawRate=PIDReturn[2];
      //   if (inputThrottle > 1800) inputThrottle = 1800;
      //   MotorInput1= 1.024*(inputThrottle-inputRoll-inputPitch-inputYaw);
      //   MotorInput2= 1.024*(inputThrottle-inputRoll+inputPitch+inputYaw);
      //   MotorInput3= 1.024*(inputThrottle+inputRoll+inputPitch-inputYaw);
      //   MotorInput4= 1.024*(inputThrottle+inputRoll-inputPitch+inputYaw);
      //   if (MotorInput1 > 2000)MotorInput1 = 1999;
      //   if (MotorInput2 > 2000)MotorInput2 = 1999; 
      //   if (MotorInput3 > 2000)MotorInput3 = 1999; 
      //   if (MotorInput4 > 2000)MotorInput4 = 1999;
      //   int ThrottleIdle=1180;
      //   if (MotorInput1 < ThrottleIdle) MotorInput1 = ThrottleIdle;
      //   if (MotorInput2 < ThrottleIdle) MotorInput2 = ThrottleIdle;
      //   if (MotorInput3 < ThrottleIdle) MotorInput3 = ThrottleIdle;
      //   if (MotorInput4 < ThrottleIdle) MotorInput4 = ThrottleIdle;
      //   int ThrottleCutOff=1000;
      //   if (ReceiverValue[2]<1050) {
      //     MotorInput1=ThrottleCutOff; 
      //     MotorInput2=ThrottleCutOff;
      //     MotorInput3=ThrottleCutOff; 
      //     MotorInput4=ThrottleCutOff;
      //     reset_pid();
      //   }
      //   analogWrite(1,MotorInput1);
      //   analogWrite(2,MotorInput2);
      //   analogWrite(3,MotorInput3); 
      //   analogWrite(4,MotorInput4);
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