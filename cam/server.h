#ifndef _SERVER_H_
#define _SERVER_H_

#include <WiFi.h>
#include "esp_camera.h"
#include "data.h"

namespace com {
  namespace config {
    inline constexpr int PORT = 5000;
    inline constexpr int SEND_FAIL_TIMEOUT = 10;
  }
  
  class TcpServer {
  private:
    WiFiServer server;
    WiFiClient client;
    data::ControlData currentControlData;
    char controlDataBuffer[sizeof(data::ControlData)];

    bool sendall(const char* buffer, size_t size) {
      size_t sent = 0;
      int times = 0;
      while (sent < size) {
        size_t currentSent = client.write(buffer + sent, size - sent);
        if (currentSent == 0) {
          times++;
          if (times >= config::SEND_FAIL_TIMEOUT)
            return false;
        } else
          times = 0;
        sent += currentSent;
      }

      return true;
    }

  public:
    TcpServer(): server(config::PORT) {
      server.begin();
    }

    void update(const data::SensorData& sensorData) {
      if (!client) {
        client = server.available();
        return;
      }

      if (!client.connected()) {
        client.stop();
        return;
      }

      if (client.available() >= sizeof(data::ControlData)) {
        for (int i = 0; i < sizeof(data::ControlData); i++)
          controlDataBuffer[i] = client.read();
        currentControlData = data::ControlData::binaryDeserialize(controlDataBuffer);
      }

      // sendinggg
      camera_fb_t* frame = esp_camera_fb_get();
      if (!frame) {
        Serial.println("ten cuidado con la gente");
        return;
      }

      size_t networkLen = frame->len;
      sendall((char*)(&frame->len), sizeof(size_t));
      sendall((char*)frame->buf, networkLen);
      esp_camera_fb_return(frame);

      String sensorBuffer = data::SensorData::serialize(sensorData);
      unsigned int sensorBufferLength = sensorBuffer.length();
      if (!sendall((char*)(&sensorBufferLength), sizeof(unsigned int))) {
        client.stop();
        return;
      }
      if (!sendall(sensorBuffer.c_str(), sensorBufferLength)) {
        client.stop();
        return;
      }
    }

    const data::ControlData& getInput() const {
      return currentControlData;
    }
  };
}

#endif