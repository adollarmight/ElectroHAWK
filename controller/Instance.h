#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <string>
#include "Hardware.h"
#include "Communication.h"

namespace Config {
  inline constexpr int RX = 16;
  inline constexpr int TX = 17;
  inline constexpr int RTS = 12;
  inline constexpr int CTS = 14;
}

class Instance {
private:
  Communication::UART<
    Communication::SensorData, 
    Communication::DirectionData
    > communication;

public:
  Instance(): communication(Config::RX, Config::TX) {
    Serial.begin(115200);
    Hardware::initialize(); 
  }

  ~Instance() {
  }

  void update() {
    communication.send({ 100.0, 100.0, 100.0, 100.0, 100.0, 100.0, 100.0 });
    if (communication.receivedDataCount()) {
      Communication::DirectionData direction = communication.recieve();
      Serial.print(direction.direction);
      Serial.print(" ");
      Serial.println(direction.magnitude);
    }
    delay(100);
  }
};

#endif