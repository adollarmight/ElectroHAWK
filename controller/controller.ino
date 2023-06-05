#include "hardware.h"
#include "data.h"
#include "uart.h"
#include "driver.h"

#define RX 16
#define TX 17

using UartType = com::Uart<data::ControlData, data::SensorData>;

HardwareSerial uartSerial(2);

std::unique_ptr<Control::Driver> driver;
std::unique_ptr<UartType> uart;

void setup() {
  Serial.begin(115200);
  uartSerial.begin(115200, SERIAL_8N1, RX, TX);
  uart = std::unique_ptr<UartType>(new UartType(uartSerial));

  driver = std::unique_ptr<Control::Driver>(new Control::Driver());
}

void loop() {
  if (uart->isReadyToReceive()) {
      data::ControlData controlData = uart->getReceived();
      driver->setControls(controlData);
      Serial.println(controlData.xAxis);
  }
  uart->send(driver->getSensorData());
  uart->update();
}
