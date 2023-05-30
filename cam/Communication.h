#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include <SoftwareSerial.h>
#include <thread>
#include <mutex>
#include <WiFi.h>
#include "esp_camera.h"
#include "Data.h"

#define htonll(x) ((1==htonl(1)) ? (x) : (((uint64_t)htonl((x) & 0xFFFFFFFFUL)) << 32) | htonl((uint32_t)((x) >> 32)))
#define ntohll(x) ((1==ntohl(1)) ? (x) : (((uint64_t)ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((uint32_t)((x) >> 32)))

#define TX 1
#define RX 3

class UartCommunication {
private:
  SoftwareSerial board;
  bool running = true, readyToReceive = false, readyToSend = false;
  SensorData received;
  char* sendBuffer;

  std::thread receiveThread;
  std::thread sendThread;
  std::mutex handleMutex;

  void receiveHandle() {
    while (running) {
      if (board.available() >= sizeof(SensorData)) {
        char* buffer = new char[sizeof(SensorData)];
        for (int i = 0; i < sizeof(SensorData); i++)
          buffer[i] = board.read();
        handleMutex.lock();
        received = SensorData::deserialize(buffer);
        readyToReceive = true;
        handleMutex.unlock();
        delete buffer;
      }
      delay(100);
    }
  }

  void sendHandle() {
    while (running) {
      if (readyToSend) {
        handleMutex.lock();
        board.write(sendBuffer);
        delete sendBuffer;
        readyToSend = false;
        handleMutex.unlock();
      }
      delay(100);
    }
  }

public:
  UartCommunication(): board(RX, TX) {
    pinMode(RX, INPUT);
    pinMode(TX, OUTPUT);
    board.begin(115200);

    receiveThread = std::thread(&UartCommunication::receiveHandle, this);
    sendThread = std::thread(&UartCommunication::sendHandle, this);
  }

  ~UartCommunication() {
    handleMutex.lock();
    running = false;
    handleMutex.unlock();
    receiveThread.join();
    sendThread.join();
  }

  SensorData receive() {
    while (!readyToReceive)
      delay(100);
    handleMutex.lock();
    SensorData data = received;
    readyToReceive = false;
    handleMutex.unlock();
    return data;
  }

  void send(InputData data) {
    while (readyToSend)
      delay(100);
    handleMutex.lock();
    sendBuffer = InputData::serialize(&data);
    readyToSend = true;
    handleMutex.unlock();
  }
};

class TcpCommunication {
private:
  WiFiServer server;

public:
  TcpCommunication(): server(5000) {
    server.begin();
  }

  void update() {
    WiFiClient client = server.available();
    char readBuffer[sizeof(uint64_t) * 4];
    if (client) {
      while (client.available() > 0) {
        camera_fb_t* fb = esp_camera_fb_get();
        size_t size = fb->len;
        client.write((char*)(&size), sizeof(size_t));
        client.write(fb->buf, fb->len);
        esp_camera_fb_return(fb);

        client.read((uint8_t*)readBuffer, sizeof(uint64_t) * 4);
        // process the buffer
      }
    }
  }
};

#endif