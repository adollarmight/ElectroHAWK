#ifndef _DATA_H_
#define _DATA_H_

#include <sstream>

namespace data {
  struct ControlData {
    int xAxis = 0, yAxis = 0, zAxis = 0, rotation = 0;

    static String serialize(const ControlData& data) {
      String buffer = "x" + String(data.xAxis) + " " + String(data.yAxis) +
        " " + String(data.zAxis) + " " + String(data.rotation);

      return buffer;
    }

    static ControlData deserialize(String buffer) {
      ControlData controlData;

      int numbers[4];
      int index = 0;
      while (index < 4) {
        int spaceIndex = buffer.indexOf(' ');

        String numberStr = buffer.substring(0, spaceIndex);
        buffer = buffer.substring(spaceIndex + 1);

        numbers[index] = numberStr.toInt();

        index++;
      }

      controlData.xAxis = numbers[0];
      controlData.yAxis = numbers[1];
      controlData.zAxis = numbers[2];
      controlData.rotation = numbers[3];
      
      return controlData;
    }

    static ControlData binaryDeserialize(const char* buffer) {
      ControlData controlData;
      memcpy(&controlData.xAxis, (void*)buffer, sizeof(int));
      memcpy(&controlData.yAxis, (void*)buffer + sizeof(int), sizeof(int));
      memcpy(&controlData.zAxis, (void*)buffer + sizeof(int) * 2, sizeof(int));
      memcpy(&controlData.rotation, (void*)buffer + sizeof(int) * 3, sizeof(int));
      return controlData;
    }
  };

  struct SensorData {
    double pitchAngle, rollAngle, voltage, temperature, pressure, altitude;

    static String serialize(const SensorData& data) {
      String buffer = "x" + String((int)(data.pitchAngle * 100)) + " " + String((int)(data.rollAngle * 100)) +
        " " + String((int)(data.voltage * 100)) + " " + String((int)(data.temperature * 100)) + 
        " " + String((int)(data.pressure * 100)) + " " + String((int)(data.altitude * 100)); 

      return buffer;
    }

    static SensorData deserialize(String buffer) {
      SensorData sensorData;

      int numbers[6];
      int index = 0;
      while (index < 6) {
        int spaceIndex = buffer.indexOf(' ');

        String numberStr = buffer.substring(0, spaceIndex);
        buffer = buffer.substring(spaceIndex + 1);

        numbers[index] = numberStr.toInt();

        index++;
      }

      sensorData.pitchAngle = (double)numbers[0] / 100;
      sensorData.rollAngle = (double)numbers[1] / 100;
      sensorData.voltage = (double)numbers[2] / 100;
      sensorData.temperature = (double)numbers[3] / 100;
      sensorData.pressure = (double)numbers[4] / 100;
      sensorData.altitude = (double)numbers[5] / 100;
      
      return sensorData;
    }
  };
}

#endif