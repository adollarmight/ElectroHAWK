using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ElectroHAWK {
    public struct SensorData {
        public double PitchAngle, RollAngle, Voltage, Temperature, Pressure, Altitude;

        public SensorData(
            double pitchAngle, 
            double rollAngle, 
            double voltage, 
            double temperature, 
            double pressure, 
            double altitude
            ) {
            this.PitchAngle = pitchAngle;
            this.RollAngle = rollAngle;
            this.Voltage = voltage;
            this.Temperature = temperature;
            this.Pressure = pressure;
            this.Altitude = altitude;
        }

        public static SensorData Parse(string buffer) {
            SensorData sensorData = new SensorData();
            string[] values = buffer.Substring(1).Split(' ');
            values[0].Remove(0);

            sensorData.PitchAngle = (double)int.Parse(values[0]) / 100;
            sensorData.RollAngle = (double)int.Parse(values[1]) / 100;
            sensorData.Voltage = (double)int.Parse(values[2]) / 100;
            sensorData.Temperature = (double)int.Parse(values[3]) / 100;
            sensorData.Pressure = (double)int.Parse(values[4]) / 100;
            sensorData.Altitude = (double)int.Parse(values[5]) / 100;
            return sensorData;
        }
    }
}
