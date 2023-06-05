#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include "hardware.h"
#include "data.h"
#include "uart.h"
#include "driver.h"

namespace Config {
  inline constexpr int RX = 16;
  inline constexpr int TX = 17;
  inline constexpr int RTS = 12;
  inline constexpr int CTS = 14;
}

class Instance {
private:
  HardwareSerial uartSerial;

  std::unique_ptr<Control::Driver> driver;
  std::unique_ptr<com::Uart<data::ControlData, data::SensorData>> uart;

public:
  Instance():
    uartSerial(2),
    driver(new Control::Driver),
    uart(new com::Uart<data::ControlData, data::SensorData>(uartSerial))
  {
    Serial.begin(115200);
    uartSerial.begin(115200, SERIAL_8N1, RX, TX);
  }

  ~Instance() {

  }

  void update() {
    if (uart->isReadyToReceive()) {
      data::ControlData controlData = uart->getReceived();
      driver->setControls(controlData);
    }
    uart->send(driver->getSensorData());
    uart->update();
  }
};

#endif