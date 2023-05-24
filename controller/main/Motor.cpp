#include "Motor.h"
#include <cmath>
#include "freertos/task.h"

namespace Hardware {
    MotorImpl::MotorImpl(gpio_num_t pin): Motor() {
        controller.attach(pin);
    }

    void MotorImpl::setSpeed(double speed) {
        if (speed > 1) 
            speed = 1;
        else if (speed < 0)
            speed = 0;
        controller.write(std::floor(speed * 180));
    }
}