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
    public partial class Connect : Form
    {
        public Connect()
        {
            InitializeComponent();
        }

        private void ConnectButton_Click(object sender, EventArgs e)
        {
            try
            {
                var connection = new Connection(IpAddressTextBox.Text);
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

        private void Connect_FormClosed(object sender, FormClosedEventArgs e) {
            Application.Exit();
        }
    }
}
