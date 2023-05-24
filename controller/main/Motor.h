#ifndef _MOTOR_H_
#define _MOTOR_H_

#include <utility>
#include "driver/gpio.h"
#include "servoControl.h"

namespace Hardware {
    class Motor {
    public:
        virtual void setSpeed(double speed);
    };

    inline constexpr std::pair<int, int> PULSE_WIDTH = { 1000, 2000 };

    class MotorImpl: public Motor {
    private:
        servoControl controller;

    public:
        MotorImpl(gpio_num_t pin);
        void setSpeed(double speed) override;
    };

    namespace Integration {
        inline void halfSpeed(gpio_num_t pin = GPIO_NUM_25) {
            Motor* motor = new MotorImpl(pin);
            while (1) {
                motor->setSpeed(1);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                motor->setSpeed(0.5);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
                motor->setSpeed(0);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
        }
    }
}

#endif