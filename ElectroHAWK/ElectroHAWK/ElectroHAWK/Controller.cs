using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ElectroHAWK
{
    public partial class Controller : Form
    {
        private Connection connection;
        private Connect last;

        public Controller(Connection connection, Connect last)
        {
            InitializeComponent();
            this.connection = connection;
            this.last = last;
            this.connection.ImageChanged += (sender, image) =>
            {
                Video.Image = image;
            };
            this.connection.SensorDataChanged += (sender, sensor) => {
                Invoke(new Action(() =>
                {
                    PitchAngleLabel.Text = "Pitch: " + sensor.PitchAngle.ToString() + "°";
                    RollAngleLabel.Text = "Roll: " + sensor.RollAngle.ToString() + "°";
                    VoltageLabel.Text = "Battery: " + sensor.Voltage.ToString() + "V";
                    // TemperatureLabel.Text = "Temperature: " + sensor.Temperature.ToString() + "°C";
                    // PressureLabel.Text = "Pressure: " + sensor.Pressure.ToString() + "hPa";
                    // AltitudeLabel.Text = "Altitude: " + sensor.Altitude.ToString() + "m";
                }));
            };
            PitchAngleLabel.BackColor = System.Drawing.Color.Transparent;
        }

        private void ZAxisTrackBar_Scroll(object sender, EventArgs e) {
            InputData input = connection.CurrentInput;
            input.ZAxis = ZAxisTrackBar.Value;
            connection.CurrentInput = input;
        }

        private void XAxisInput_Scroll(object sender, EventArgs e) {
            InputData input = connection.CurrentInput;
            input.XAxis = XAxisInput.Value;
            connection.CurrentInput = input;
        }

        private void RotationTrackBar_Scroll(object sender, EventArgs e) {
            InputData input = connection.CurrentInput;
            input.Rotation = RotationTrackBar.Value;
            connection.CurrentInput = input;
        }

        private void Controller_FormClosing(object sender, FormClosingEventArgs e)
        {
            connection.Running = false;
            Application.Exit();
        }
        private void Controller_FormClosed(object sender, FormClosedEventArgs e) {
            connection.Running = false;
            Application.Exit();
        }
    }
}
