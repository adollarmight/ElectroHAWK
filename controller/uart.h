#ifndef _UARTH_
#define _UARTH_

#include <stdexcept>

namespace com {
  template<typename ReceiveData, typename SendData>
  class Uart {
  private:
    HardwareSerial& serial;
    ReceiveData received;
    bool readyToReceive = false;

  public:
    Uart(HardwareSerial& serial): serial(serial) {}

    void update() {
      if (serial.available() && serial.read() == 'x') {
        serial.write("b");
        String buffer;
        while (serial.available()) {
          char c = serial.read();
          buffer.concat(c);
        }
        received = ReceiveData::deserialize(buffer);
        readyToReceive = true;
      }
    }

    const bool& isReadyToReceive() const {
      return readyToReceive;
    }

    const ReceiveData getReceived() {
      if (!readyToReceive)
        throw std::runtime_error("data wasn't received");
      readyToReceive = false;
      return received;
    }

    void send(const SendData& data) {
      String buffer = SendData::serialize(data);
      serial.write(buffer.c_str());
    }
  };
}

#endif