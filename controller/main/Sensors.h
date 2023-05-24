#ifndef _SENSORS_H_
#define _SENSORS_H_

#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_system.h"

#include "MPU6050_6Axis_MotionApps20.h"
#include "I2Cdev.h"

#include <iostream>
#include <thread> // modern C++ > freertos (we richhh)
#include <mutex>

namespace Hardware {
    namespace Config {
        inline constexpr unsigned int I2CBandwidth = 400000;
        namespace Gyro {
            inline constexpr i2c_port_t I2C = I2C_NUM_0;
            inline constexpr gpio_num_t SDA = GPIO_NUM_21;
            inline constexpr gpio_num_t SCL = GPIO_NUM_22; 
            inline constexpr uint16_t packetSize = 42;
        }
    }

    // building the MPU6050 class using the singlebitch pattern
    class Gyroscope {
    private:
        I2Cdev i2c;
        MPU6050 mpu;
        std::thread readThread;
        std::mutex readMutex;
        double yaw, pitch, roll;
        bool run = true;


        void readThreadHandle();

    public:
        Gyroscope(
            i2c_port_t port = Config::Gyro::I2C, 
            gpio_num_t sda = Config::Gyro::SDA, 
            gpio_num_t scl = Config::Gyro::SCL
            );
        ~Gyroscope();

        double getYaw() const;
        double getPitch() const;
        double getRoll() const; 
    };

    namespace Integration {
        inline void gyroReading() {
            Hardware::Gyroscope gyro;
            while (true) {
                printf(
                    "Yaw: %lf Pitch: %lf Roll: %lf\n", 
                    gyro.getYaw(), gyro.getPitch(), gyro.getRoll()
                    );
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
        }
    }
}

#endif