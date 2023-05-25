#include <mutex>
#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include <queue>
#include <thread>
#include <mutex>
#include <cstring>
#include <arpa/inet.h>
#include <sys/types.h>
#include <SoftwareSerial.h>
#include "Direction.h"

// 64 bit host/network convertions
#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#define ntohll(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))

// establish communication with the other MCU
namespace Communication {
  namespace Config {
    inline constexpr int baudRate = 115200;
  }

  // pretty self explanatory
  inline bool isBigEndian()
  {
      union {
          uint32_t i;
          char c[4];
      } bint = {0x01020304};

      return bint.c[0] == 1;
  }

  // double de/serialization works by transforming the double into a uint64_t and using hton
  inline uint64_t serializeDouble(const double data) {
    uint64_t networkData;
    memcpy(&networkData, &data, sizeof(double));
    networkData = htonll(networkData);
    return networkData;
  }

  inline double deserializeDouble(const void* data) {
    uint64_t hostData;
    memcpy(&hostData, data, sizeof(uint64_t));
    hostData = ntohll(hostData);
    double ddata;
    memcpy(&ddata, &hostData, sizeof(double));
    return ddata;
  }

  template<typename T>
  struct Serializable {
    virtual char* serialize() = 0;
    static virtual T* deserialize(const char* binary) const = 0;
  }

  // receive
  struct DirectionData: public Serializable<DirectionData> {
    Driver::Direction direction;
    double magnitude;

    /*
      Serialization works as follows:
        big endian int - representing direction
          * Front - 0
          * Back - 1
          * Left - 2
          * Right - 3
          * Up - 4
          * Down - 5
          * RotateLeft - 6
          * RotateRight - 7
          * Rest - 8
          * Nothing - handled by itself
        big endian uint64_t - representing magnitude
    */
    char* serialize() {
      char* buffer = new char[sizeof(int) + sizeof(double)];
      int networkDirection = htonl(static_cast<int>(direction));
      uint64_t networkMagnitude;
      memcpy(&networkMagnitude, &magnitude, sizeof(double));
      memcpy(buffer, &htonll(networkMagnitude), sizeof(int));
      memcpy(buffer + sizeof(int), &networkMagnitude, sizeof(double));
    }

    static DirectionData* deserialize(const char* binary) const {
      DirectionData* data = new DirectionData();
      int idirection;
      memcpy(&idirection, binary, sizeof(int));
      idirection = ntohl(idirection);
      switch (idirection) {
        case 0:
          data->direction = Movement::Front;
          break;
        case 1:
          data->direction = Movement::Back;
          break;
        case 2:
          data->direction = Movement::Left;
          break;
        case 3:
          data->direction = Movement::Right;
          break;
        case 4:
          data->direction = Movement::Up;
          break;
        case 5:
          data->direction = Movement::Down;
          break;
        case 6:
          data->direction = Movement::Rest;
          break;
        case 7:
          data->direction = Movement::RotateLeft;
          break;
        case 8:
          data->direction = Movement::RotateRight;
          break;
      }

      data->magnitude = deserializeDouble(binary + sizeof(int));
    }
  };

  // send
  struct SensorData: public Seserializable<SensorData> {
    double temperature;
    double voltage;
    double yaw, pitch, roll;
    double pressure;
    double attitude;

    char* serialize() {
      char* buffer = new char[sizeof(double) * 7];
      memcpy(buffer, &serializeDouble(temperature), sizeof(double));
      memcpy(buffer + 8, &serializeDouble(voltage), sizeof(double));
      memcpy(buffer + 16, &serializeDouble(yaw), sizeof(double));
      memcpy(buffer + 24, &serializeDouble(pitch), sizeof(double));
      memcpy(buffer + 32, &serializeDouble(roll), sizeof(double));
      memcpy(buffer + 40, &serializeDouble(pressure), sizeof(double));
      memcpy(buffer + 48, &serializeDouble(attitude), sizeof(double));
      return buffer;
    }

    static SensorData* deserialize(char* buffer) {
      SensorData* data = new SensorData();
      uint64_t ntemperature, nvoltage, nyaw, npitch, nroll, npressure, nattitude;
      memcpy(&ntemperature, buffer, sizeof(uint64_t));
      memcpy(&nvoltage, buffer + sizeof(uint64_t), sizeof(uint64_t));
      memcpy(&nyaw, buffer + sizeof(uint64_t) * 2, sizeof(uint64_t));
      memcpy(&npitch, buffer + sizeof(uint64_t) * 3, sizeof(uint64_t));
      memcpy(&nroll, buffer + sizeof(uint64_t) * 4, sizeof(uint64_t));
      memcpy(&npressure, buffer + sizeof(uint64_t) * 5, sizeof(uint64_t));
      memcpy(&nattitude, buffer + sizeof(uint64_t) * 6, sizeof(uint64_t));
      memcpy(&data->temperature, &ntohll(ntemperature), sizeof(double));
      memcpy(&data->voltage, &ntohll(nvoltage), sizeof(double));
      memcpy(&data->yaw, &ntohll(nyaw), sizeof(double));
      memcpy(&data->pitch, &ntohll(npitch), sizeof(double));
      memcpy(&data->roll, &ntohll(nroll), sizeof(double));
      memcpy(&data->pressure, &ntohll(npressure), sizeof(double));
      memcpy(&data->attitude, &ntohll(nattitude), sizeof(double));
    }
  };
  
  // todo implement: CTS and RTS
  template<typename R, typename S>
  class UART {
  private:
    std::queue<R> receivedData;
    std::queue<S> sendData;

    SoftwareSerial boardSerial;
    bool running = true;
    std::thread communicationThread;
    std::mutex dataMutex;

    // sends and receives data
    void communicationHandle() {
      while (running) {
        if (boardSerial.available() >= sizeof(R)) {
          char* data = new char[sizeof(R)];
          receivedData.push(R::deserialize(data));
        }

        if (!sendData.empty()) {
          S data = sendData.front(); sendData.pop();
          char* buffer = data.serialize();
          boardSerial.write(buffer);
          delete buffer;
        }
      }
    }

  public:
    UART(int rx, int tx): boardSerial(rx, tx) {
      boardSerial.begin(Config::baudRate);
    }

    ~UART() {
      dataMutex.lock();
      running = false;
      dataMutex.unlock();
      communicationThread.join();
    }

    std::size_t receivedDataCount() {
      return receivedData.size();
    }

    // blocking
    R receive() {
      while (receivedData.empty()) {}
      R data = receivedData.front(); 
      dataMutex.lock();
      receivedData.pop();
      dataMutex.unlock();
      return data;
    }

    void send(S& data) {
      dataMutex.lock();
      sendData.push(data);
      dataMutex.unlock();
    }
  }
}

#endif