using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ElectroHAWK
{
    public class Connection
    {
        private static readonly int Port = 5000;
        private readonly Socket Sock;

        private readonly object CurrentImageLock = new object();
        public Image CurrentImage { get; set; }
        public event EventHandler<Image> ImageChanged;

        private readonly object CurrentInputLock = new object();
        private InputData currentInput = new InputData();
        public InputData CurrentInput {
            get { return currentInput; }
            set {
                lock (CurrentInputLock) {
                    this.currentInput = value;
                }
            }
        }

        private readonly object CurrentSensorDataLock = new object();
        private SensorData currentSensorData;
        public event EventHandler<SensorData> SensorDataChanged;

        private bool running = true;
        public bool Running 
        {
            get { return running; }
            set 
            { 
                lock(CurrentImageLock) 
                {
                    this.running = value;
                }
            }
        }

        private readonly Thread thread;

        public Connection(String ipAddress)
        {
            IPAddress address = IPAddress.Parse(ipAddress);
            IPEndPoint endPoint = new IPEndPoint(address, Port);
            Sock = new Socket(
                address.AddressFamily, 
                SocketType.Stream, 
                ProtocolType.Tcp
                );
            Sock.Connect(endPoint);
            thread = new Thread(HandleThread);
            thread.Start();
        }

        public void HandleThread() {
            try {
                while (running) {
                    // sending
                    Sock.Send(BitConverter.GetBytes(CurrentInput.XAxis));
                    Sock.Send(BitConverter.GetBytes(CurrentInput.YAxis));
                    Sock.Send(BitConverter.GetBytes(CurrentInput.ZAxis));
                    Sock.Send(BitConverter.GetBytes(CurrentInput.Rotation));

                    // receiving (dr. dre be the name still runnin the game)
                    byte[] sizeArray = new byte[4];
                    Sock.Receive(sizeArray, 4, SocketFlags.None);
                    int size = BitConverter.ToInt32(sizeArray, 0);
                    byte[] buffer = new byte[size];
                    Sock.Receive(buffer, size, (SocketFlags)0x8);
                    using (MemoryStream memoryStream = new MemoryStream(buffer)) {
                        CurrentImage = Image.FromStream(memoryStream);
                        ImageChanged?.Invoke(this, CurrentImage);
                    }

                    // now for sensor data
                    Sock.Receive(sizeArray, 4, SocketFlags.None);
                    size = BitConverter.ToInt32(sizeArray, 0);
                    buffer = null; // dispose
                    buffer = new byte[size];
                    Sock.Receive(buffer, size, (SocketFlags)0x8);
                    try {
                        currentSensorData = SensorData.Parse(ASCIIEncoding.ASCII.GetString(buffer));
                        SensorDataChanged?.Invoke(this, currentSensorData);
                    } catch { }
                }
            } catch {
                MessageBox.Show(
                    "Fatal error, am plecat papa",
                    "Eroare majora sa moara sa intepeneasca",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error
                    );
                Application.Exit();
            }
        }
    }
}
