#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include <queue>
#include <cstring>
#include <arpa/inet.h>
#include <sys/types.h>
#include "Direction.h"

// 64 bit host/network convertions
#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
#define ntohll(x) ((1==ntohl(1)) ? (x) : ((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | ntohl((x) >> 32))

namespace Communication {
  namespace Config {
    inline constexpr RX = 16;
    inline constexpr TX = 17;
    inline constexpr RTS = 12;
    inline constexpr CTS = 14;
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

  struct Serializable {
    virtual char* serialize();
  }

  template<typename T>
  struct Deserializable {
    virtual T* serialize(const char* binary) const;
    virtual char* deserialize();
  }

  // receive
  struct DirectionData: public Deserializable<DirectionData> {
    Driver::Direction direction;
    double magnitude;

    DirectionData* deserialize(const char* binary) const {
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

      DirectionData* data = new DirectionData();
      int idirection;
      memcpy(&idirection, binary, sizeof(int));
      idirection = ntohl(idirection);
      switch (idirection) {
        case 0:
          data->direction = Front;
          break;
        case 1:
          data->direction = Back;
          break;
        case 2:
          data->direction = Left;
          break;
        case 3:
          data->direction = Right;
          break;
        case 4:
          data->direction = Up;
          break;
        case 5:
          data->direction = Down;
          break;
        case 6:
          data->direction = Rest;
          break;
        case 7:
          data->direction = RotateLeft;
          break;
        case 8:
          data->direction = RotateRight;
          break;
      }

      data->magnitude = deserializeDouble(binary + sizeof(int));
    }
  };

  // send
  struct SensorData: public Seserializable {
    double temperature;
    double voltage;
    double yaw, pitch, roll;
    double pressure;
    double attitude;

    char* serialize() {
      // for each field we have an uint64_t
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
  };
  
  class UART {
  private:
    std::queue<DirectionData> receivedData;
    std::queue<SensorData> sendData;

    

  public:


    DirectionData receive();
    void send(SensorData data);
  }
}

#endif