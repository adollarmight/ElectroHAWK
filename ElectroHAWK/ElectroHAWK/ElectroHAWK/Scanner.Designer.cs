namespace ElectroHAWK
{
    partial class Scanner
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.MainLayout = new System.Windows.Forms.TableLayoutPanel();
            this.InputLayout = new System.Windows.Forms.TableLayoutPanel();
            this.SubnetLayout = new System.Windows.Forms.FlowLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.SubnetInput = new System.Windows.Forms.TextBox();
            this.ButtonLayout = new System.Windows.Forms.FlowLayoutPanel();
            this.ScanButton = new System.Windows.Forms.Button();
            this.StartButton = new System.Windows.Forms.Button();
            this.IPList = new System.Windows.Forms.DataGridView();
            this.Address = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.MainLayout.SuspendLayout();
            this.InputLayout.SuspendLayout();
            this.SubnetLayout.SuspendLayout();
            this.ButtonLayout.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.IPList)).BeginInit();
            this.SuspendLayout();
            // 
            // MainLayout
            // 
            this.MainLayout.ColumnCount = 1;
            this.MainLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.MainLayout.Controls.Add(this.InputLayout, 0, 1);
            this.MainLayout.Controls.Add(this.IPList, 0, 0);
            this.MainLayout.Dock = System.Windows.Forms.DockStyle.Fill;
            this.MainLayout.Location = new System.Drawing.Point(0, 0);
            this.MainLayout.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.MainLayout.Name = "MainLayout";
            this.MainLayout.RowCount = 2;
            this.MainLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.MainLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 78F));
            this.MainLayout.Size = new System.Drawing.Size(1023, 540);
            this.MainLayout.TabIndex = 0;
            // 
            // InputLayout
            // 
            this.InputLayout.ColumnCount = 2;
            this.InputLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.InputLayout.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.InputLayout.Controls.Add(this.SubnetLayout, 0, 0);
            this.InputLayout.Controls.Add(this.ButtonLayout, 1, 0);
            this.InputLayout.Dock = System.Windows.Forms.DockStyle.Fill;
            this.InputLayout.Location = new System.Drawing.Point(3, 464);
            this.InputLayout.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.InputLayout.Name = "InputLayout";
            this.InputLayout.RowCount = 1;
            this.InputLayout.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.InputLayout.Size = new System.Drawing.Size(1017, 74);
            this.InputLayout.TabIndex = 0;
            // 
            // SubnetLayout
            // 
            this.SubnetLayout.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.SubnetLayout.AutoSize = true;
            this.SubnetLayout.Controls.Add(this.label1);
            this.SubnetLayout.Controls.Add(this.SubnetInput);
            this.SubnetLayout.Location = new System.Drawing.Point(116, 24);
            this.SubnetLayout.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.SubnetLayout.Name = "SubnetLayout";
            this.SubnetLayout.Size = new System.Drawing.Size(276, 26);
            this.SubnetLayout.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 6);
            this.label1.Margin = new System.Windows.Forms.Padding(3, 6, 3, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(52, 16);
            this.label1.TabIndex = 0;
            this.label1.Text = "Subnet:";
            // 
            // SubnetInput
            // 
            this.SubnetInput.Location = new System.Drawing.Point(61, 2);
            this.SubnetInput.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.SubnetInput.Name = "SubnetInput";
            this.SubnetInput.Size = new System.Drawing.Size(212, 22);
            this.SubnetInput.TabIndex = 1;
            this.SubnetInput.Text = "192.168.1";
            // 
            // ButtonLayout
            // 
            this.ButtonLayout.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.ButtonLayout.AutoSize = true;
            this.ButtonLayout.Controls.Add(this.ScanButton);
            this.ButtonLayout.Controls.Add(this.StartButton);
            this.ButtonLayout.Location = new System.Drawing.Point(654, 19);
            this.ButtonLayout.Margin = new System.Windows.Forms.Padding(4);
            this.ButtonLayout.Name = "ButtonLayout";
            this.ButtonLayout.Size = new System.Drawing.Size(216, 36);
            this.ButtonLayout.TabIndex = 1;
            // 
            // ScanButton
            // 
            this.ScanButton.Location = new System.Drawing.Point(4, 4);
            this.ScanButton.Margin = new System.Windows.Forms.Padding(4);
            this.ScanButton.Name = "ScanButton";
            this.ScanButton.Size = new System.Drawing.Size(100, 28);
            this.ScanButton.TabIndex = 0;
            this.ScanButton.Text = "Scan";
            this.ScanButton.UseVisualStyleBackColor = true;
            this.ScanButton.Click += new System.EventHandler(this.ScanButton_Click);
            // 
            // StartButton
            // 
            this.StartButton.Enabled = false;
            this.StartButton.Location = new System.Drawing.Point(112, 4);
            this.StartButton.Margin = new System.Windows.Forms.Padding(4);
            this.StartButton.Name = "StartButton";
            this.StartButton.Size = new System.Drawing.Size(100, 28);
            this.StartButton.TabIndex = 1;
            this.StartButton.Text = "Start";
            this.StartButton.UseVisualStyleBackColor = true;
            this.StartButton.Click += new System.EventHandler(this.StartButton_Click);
            // 
            // IPList
            // 
            this.IPList.AllowUserToAddRows = false;
            this.IPList.AllowUserToDeleteRows = false;
            this.IPList.AllowUserToResizeColumns = false;
            this.IPList.AllowUserToResizeRows = false;
            this.IPList.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.IPList.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Address});
            this.IPList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.IPList.Location = new System.Drawing.Point(27, 25);
            this.IPList.Margin = new System.Windows.Forms.Padding(27, 25, 27, 25);
            this.IPList.MultiSelect = false;
            this.IPList.Name = "IPList";
            this.IPList.RowHeadersWidth = 51;
            this.IPList.Size = new System.Drawing.Size(969, 412);
            this.IPList.TabIndex = 1;
            this.IPList.RowEnter += new System.Windows.Forms.DataGridViewCellEventHandler(this.IPList_RowEnter);
            // 
            // Address
            // 
            this.Address.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.Address.HeaderText = "Address";
            this.Address.MinimumWidth = 6;
            this.Address.Name = "Address";
            this.Address.ReadOnly = true;
            // 
            // Scanner
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1023, 540);
            this.Controls.Add(this.MainLayout);
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Scanner";
            this.Text = "Scanner";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Scanner_FormClosing);
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Scanner_FormClosed);
            this.MainLayout.ResumeLayout(false);
            this.InputLayout.ResumeLayout(false);
            this.InputLayout.PerformLayout();
            this.SubnetLayout.ResumeLayout(false);
            this.SubnetLayout.PerformLayout();
            this.ButtonLayout.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.IPList)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TableLayoutPanel MainLayout;
        private System.Windows.Forms.TableLayoutPanel InputLayout;
        private System.Windows.Forms.FlowLayoutPanel SubnetLayout;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox SubnetInput;
        private System.Windows.Forms.DataGridView IPList;
        private System.Windows.Forms.DataGridViewTextBoxColumn Address;
        private System.Windows.Forms.FlowLayoutPanel ButtonLayout;
        private System.Windows.Forms.Button ScanButton;
        private System.Windows.Forms.Button StartButton;
    }
}