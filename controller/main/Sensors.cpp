#include <stdexcept>
#include "Sensors.h"

namespace Hardware {
    Gyroscope::Gyroscope(i2c_port_t port, gpio_num_t sda, gpio_num_t scl): i2c(port) {
        i2c.initialize(sda, scl, Config::I2CBandwidth);
        mpu.initialize(&i2c, MPU6050_DEFAULT_ADDRESS);
        mpu.CalibrateGyro();
        mpu.CalibrateAccel();
        mpu.dmpInitialize();
        mpu.setDMPEnabled(true);

        readThread = std::thread {&Gyroscope::readThreadHandle, this};
    }

    Gyroscope::~Gyroscope()
    {
        readMutex.lock();
        run = false;
        readMutex.unlock();
        readThread.join();
    }

    void Gyroscope::readThreadHandle() {
        float ypr[3];
        uint8_t fifoBuffer[64];
        Quaternion quaternion;
        VectorFloat gravity;

        while (run) {
            uint8_t mpuIntStatus = mpu.getIntStatus();
            uint16_t fifoCount = mpu.getFIFOCount();

            if ((mpuIntStatus & 0x10) || fifoCount == 1024)
                mpu.resetFIFO();
            else if (mpuIntStatus & 0x02) {
                while (fifoCount < Config::Gyro::packetSize)
                    fifoCount = mpu.getFIFOCount();
                mpu.getFIFOBytes(fifoBuffer, Config::Gyro::packetSize);

                mpu.dmpGetQuaternion(&quaternion, fifoBuffer);
                mpu.dmpGetGravity(&gravity, &quaternion);
                mpu.dmpGetYawPitchRoll(ypr, &quaternion, &gravity);
                readMutex.lock();
                yaw = ypr[0];
                pitch = ypr[1];
                roll = ypr[2];
                readMutex.unlock();
            }
        }
    }

    double Gyroscope::getYaw() const {
        return yaw;
    }

    double Gyroscope::getPitch() const {
        return pitch;
    }

    double Gyroscope::getRoll() const {
        return roll;
    }
}