using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ElectroHAWK {
    public struct InputData {
        public int XAxis, YAxis, ZAxis, Rotation;

        public InputData(int xAxis, int yAxis, int zAxis, int rotation) {
            this.XAxis = xAxis;
            this.YAxis = yAxis;
            this.ZAxis = zAxis;
            this.Rotation = rotation;
        }
    }
}
