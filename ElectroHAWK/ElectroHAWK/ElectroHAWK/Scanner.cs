using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ElectroHAWK
{
    public partial class Scanner : Form
    {
        private Connect Last;

        public Scanner(Connect Last)
        {
            InitializeComponent();
            this.Last = Last;
        }

        private void Scanner_FormClosing(object sender, FormClosingEventArgs e)
        {
            this.Hide();
            Last.Show();
        }

        private void Scanner_FormClosed(object sender, FormClosedEventArgs e)
        {
            this.Hide();
            Last.Show();
        }

        private void ScanButton_Click(object sender, EventArgs e)
        {
            try
            {
                IPAddress.Parse(SubnetInput.Text + "." + 0.ToString());
            } catch
            {
                MessageBox.Show(
                    "Invalid subnet",
                    "Error",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error
                    );
                return;
            }
            ScanButton.Enabled = false;
            Task.Factory.StartNew(() =>
            {
                Parallel.For(0, 256, current =>
                {
                    IPAddress address;
                    if (IPAddress.TryParse(SubnetInput.Text + "." + current.ToString(), out address))
                        using (TcpClient client = new TcpClient())
                        {
                            try
                            {
                                client.Connect(address, 5000);
                                Invoke(new Action(() =>
                                {
                                    IPList.Rows.Add(address.ToString());
                                    IPList.Update();
                                }));
                            }
                            catch { }
                        }
                });
            });
        }

        private void IPList_RowEnter(object sender, DataGridViewCellEventArgs e)
        {
            StartButton.Enabled = true;
        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            try
            {
                var connection = new Connection((string)IPList.SelectedCells[0].Value);
                var controller = new Controller(connection, this);
                this.Hide();
                controller.Show();
            } catch
            {
                MessageBox.Show(
                    "Failed to connect to the given ip address",
                    "Error",
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error
                    );
                return;
            }
        }
    }
}
