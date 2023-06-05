#ifndef _SERVER_H_
#define _SERVER_H_

#include <WiFi.h>
#include "esp_camera.h"
#include "data.h"

namespace com {
  namespace config {
    inline constexpr int PORT = 5000;
  }
  
  class TcpServer {
  private:
    WiFiServer server;
    WiFiClient client;
    data::ControlData currentControlData;
    char controlDataBuffer[sizeof(data::ControlData)];

    void sendall(const char* buffer, size_t size) {
      size_t sent = 0;
      while (sent < size)
        sent += client.write(buffer + sent, size - sent);
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

      // reading
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
      sendall((char*)(&sensorBufferLength), sizeof(unsigned int));
      sendall(sensorBuffer.c_str(), sensorBufferLength);
    }

    const data::ControlData& getInput() const {
      return currentControlData;
    }
  };
}

#endif