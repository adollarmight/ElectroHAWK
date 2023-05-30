#ifndef _DATA_H_
#define _DATA_H_

#include <arpa/inet.h>
#include <cstddef>
#include <cstring>
#define htonll(x) ((1==htonl(1)) ? (x) : (((uint64_t)htonl((x) & 0xFFFFFFFFUL)) << 32) | htonl((uint32_t)((x) >> 32)))
#define ntohll(x) ((1==ntohl(1)) ? (x) : (((uint64_t)ntohl((x) & 0xFFFFFFFFUL)) << 32) | ntohl((uint32_t)((x) >> 32)))

static uint64_t serializeDouble(double x) {
  uint64_t serialized;
  memcpy(&serialized, &x, sizeof(uint64_t));
  serialized = htonll(serialized);
  return serialized;
}

static double deserializeDouble(uint64_t serialized) {
  serialized = ntohll(serialized);
  double deserialized;
  memcpy(&deserialized, &serialized, sizeof(double));
  return deserialized; 
}

struct InputData {
  double xAxis, yAxis, zAxis, rotation;
  
  static char* serialize(InputData* data) {
    char* buffer = new char[sizeof(InputData) + 1];
    uint64_t s_xAxis = serializeDouble(data->xAxis);
    uint64_t s_yAxis = serializeDouble(data->yAxis);
    uint64_t s_zAxis = serializeDouble(data->zAxis);
    uint64_t s_rotation = serializeDouble(data->rotation);
    memcpy(buffer, &s_xAxis, sizeof(uint64_t));
    memcpy(buffer + sizeof(uint64_t), &s_yAxis, sizeof(uint64_t));
    memcpy(buffer + sizeof(uint64_t) * 2, &s_zAxis, sizeof(uint64_t));
    memcpy(buffer + sizeof(uint64_t) * 3, &s_rotation, sizeof(uint64_t));
    return buffer; 
  }

  static InputData deserialize(char* buffer) {
    InputData data;
    uint64_t s_xAxis, s_yAxis, s_zAxis, s_rotation;
    memcpy(&s_xAxis, buffer, sizeof(uint64_t));
    memcpy(&s_yAxis, buffer + sizeof(uint64_t), sizeof(uint64_t));
    memcpy(&s_zAxis, buffer + sizeof(uint64_t) * 2, sizeof(uint64_t));
    memcpy(&s_rotation, buffer + sizeof(uint64_t) * 3, sizeof(uint64_t));
    data.xAxis = deserializeDouble(s_xAxis);
    data.yAxis = deserializeDouble(s_yAxis);
    data.zAxis = deserializeDouble(s_zAxis);
    data.rotation = deserializeDouble(s_rotation);
    return data;
  }
};

struct SensorData {
  double pitchAngle, rollAngle;
  double voltage;
  double pressure, temperature;

  static char* serialize(SensorData* data) {
    char* buffer = new char[sizeof(SensorData) + 1];
    uint64_t s_pitchAngle = serializeDouble(data->pitchAngle);
    uint64_t s_rollAngle = serializeDouble(data->rollAngle);
    uint64_t s_voltage = serializeDouble(data->voltage);
    uint64_t s_pressure = serializeDouble(data->pressure);
    uint64_t s_temperature = serializeDouble(data->temperature);
    memcpy(buffer, &s_pitchAngle, sizeof(uint64_t));
    memcpy(buffer + sizeof(uint64_t), &s_rollAngle, sizeof(uint64_t));
    memcpy(buffer + sizeof(uint64_t) * 2, &s_voltage, sizeof(uint64_t));
    memcpy(buffer + sizeof(uint64_t) * 3, &s_pressure, sizeof(uint64_t));
    memcpy(buffer + sizeof(uint64_t) * 4, &s_temperature, sizeof(uint64_t));
    return buffer;
  }

  static SensorData deserialize(char* buffer) {
    SensorData data;
    uint64_t s_pitchAngle, s_rollAngle, s_voltage, s_pressure, s_temperature;
    memcpy(&s_pitchAngle, buffer, sizeof(uint64_t));
    memcpy(&s_rollAngle, buffer + sizeof(uint64_t), sizeof(uint64_t));
    memcpy(&s_voltage, buffer + sizeof(uint64_t) * 2, sizeof(uint64_t));
    memcpy(&s_pressure, buffer + sizeof(uint64_t) * 3, sizeof(uint64_t));
    memcpy(&s_temperature, buffer + sizeof(uint64_t) * 4, sizeof(uint64_t));
    data.pitchAngle = deserializeDouble(s_pitchAngle);
    data.rollAngle = deserializeDouble(s_rollAngle);
    data.voltage = deserializeDouble(s_voltage);
    data.pressure = deserializeDouble(s_pressure);
    data.temperature = deserializeDouble(s_temperature);
    return data;
  }
};

#endif