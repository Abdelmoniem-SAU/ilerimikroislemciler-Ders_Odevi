/*
 * Created by SharpDevelop.
 * User: abomn
 * Date: 1/7/2026
 * Time: 3:04 AM
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
namespace MicrocontrollerProject
{
	partial class MainForm
	{
		/// <summary>
		/// Designer variable used to keep track of non-visual components.
		/// </summary>
		private System.ComponentModel.IContainer components = null;
		
		/// <summary>
		/// Disposes resources used by the form.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing) {
				if (components != null) {
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}
		
		/// <summary>
		/// This method is required for Windows Forms designer support.
		/// Do not change the method contents inside the source code editor. The Forms designer might
		/// not be able to load this method if it was changed manually.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.btnConnect = new System.Windows.Forms.Button();
			this.btnSyncTime = new System.Windows.Forms.Button();
			this.btnUpdateDisplay = new System.Windows.Forms.Button();
			this.txtPort = new System.Windows.Forms.TextBox();
			this.txtTimeIn = new System.Windows.Forms.TextBox();
			this.txtMsgIn = new System.Windows.Forms.TextBox();
			this.txtTimeOut = new System.Windows.Forms.TextBox();
			this.txtAdcOut = new System.Windows.Forms.TextBox();
			this.txtStatus = new System.Windows.Forms.TextBox();
			this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
			this.SuspendLayout();
			// 
			// btnConnect
			// 
			this.btnConnect.Location = new System.Drawing.Point(436, 42);
			this.btnConnect.Name = "btnConnect";
			this.btnConnect.Size = new System.Drawing.Size(161, 23);
			this.btnConnect.TabIndex = 0;
			this.btnConnect.Text = "btnConnect";
			this.btnConnect.UseVisualStyleBackColor = true;
			this.btnConnect.Click += new System.EventHandler(this.BtnConnectClick);
			// 
			// btnSyncTime
			// 
			this.btnSyncTime.Location = new System.Drawing.Point(436, 89);
			this.btnSyncTime.Name = "btnSyncTime";
			this.btnSyncTime.Size = new System.Drawing.Size(161, 23);
			this.btnSyncTime.TabIndex = 1;
			this.btnSyncTime.Text = "btnSyncTime";
			this.btnSyncTime.UseVisualStyleBackColor = true;
			this.btnSyncTime.Click += new System.EventHandler(this.BtnSyncTimeClick);
			// 
			// btnUpdateDisplay
			// 
			this.btnUpdateDisplay.Location = new System.Drawing.Point(436, 144);
			this.btnUpdateDisplay.Name = "btnUpdateDisplay";
			this.btnUpdateDisplay.Size = new System.Drawing.Size(161, 23);
			this.btnUpdateDisplay.TabIndex = 2;
			this.btnUpdateDisplay.Text = "btnUpdateDisplay";
			this.btnUpdateDisplay.UseVisualStyleBackColor = true;
			this.btnUpdateDisplay.Click += new System.EventHandler(this.BtnUpdateDisplayClick);
			// 
			// txtPort
			// 
			this.txtPort.Location = new System.Drawing.Point(119, 42);
			this.txtPort.Name = "txtPort";
			this.txtPort.Size = new System.Drawing.Size(100, 22);
			this.txtPort.TabIndex = 3;
			this.txtPort.Click += new System.EventHandler(this.BtnConnectClick);
			// 
			// txtTimeIn
			// 
			this.txtTimeIn.Location = new System.Drawing.Point(119, 89);
			this.txtTimeIn.Name = "txtTimeIn";
			this.txtTimeIn.Size = new System.Drawing.Size(100, 22);
			this.txtTimeIn.TabIndex = 4;
			this.txtTimeIn.Click += new System.EventHandler(this.BtnSyncTimeClick);
			// 
			// txtMsgIn
			// 
			this.txtMsgIn.Location = new System.Drawing.Point(119, 144);
			this.txtMsgIn.Name = "txtMsgIn";
			this.txtMsgIn.Size = new System.Drawing.Size(100, 22);
			this.txtMsgIn.TabIndex = 5;
			this.txtMsgIn.Click += new System.EventHandler(this.BtnUpdateDisplayClick);
			// 
			// txtTimeOut
			// 
			this.txtTimeOut.Location = new System.Drawing.Point(119, 194);
			this.txtTimeOut.Name = "txtTimeOut";
			this.txtTimeOut.Size = new System.Drawing.Size(100, 22);
			this.txtTimeOut.TabIndex = 6;
			// 
			// txtAdcOut
			// 
			this.txtAdcOut.Location = new System.Drawing.Point(119, 250);
			this.txtAdcOut.Name = "txtAdcOut";
			this.txtAdcOut.Size = new System.Drawing.Size(100, 22);
			this.txtAdcOut.TabIndex = 7;
			// 
			// txtStatus
			// 
			this.txtStatus.Location = new System.Drawing.Point(119, 292);
			this.txtStatus.Name = "txtStatus";
			this.txtStatus.Size = new System.Drawing.Size(100, 22);
			this.txtStatus.TabIndex = 8;
			// 
			// serialPort1
			// 
			this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.SerialPort1DataReceived);
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(656, 403);
			this.Controls.Add(this.txtStatus);
			this.Controls.Add(this.txtAdcOut);
			this.Controls.Add(this.txtTimeOut);
			this.Controls.Add(this.txtMsgIn);
			this.Controls.Add(this.txtTimeIn);
			this.Controls.Add(this.txtPort);
			this.Controls.Add(this.btnUpdateDisplay);
			this.Controls.Add(this.btnSyncTime);
			this.Controls.Add(this.btnConnect);
			this.Name = "MainForm";
			this.Text = "MicrocontrollerProject";
			this.ResumeLayout(false);
			this.PerformLayout();
		}
		private System.IO.Ports.SerialPort serialPort1;
		private System.Windows.Forms.TextBox txtStatus;
		private System.Windows.Forms.TextBox txtAdcOut;
		private System.Windows.Forms.TextBox txtTimeOut;
		private System.Windows.Forms.TextBox txtMsgIn;
		private System.Windows.Forms.TextBox txtTimeIn;
		private System.Windows.Forms.TextBox txtPort;
		private System.Windows.Forms.Button btnUpdateDisplay;
		private System.Windows.Forms.Button btnSyncTime;
		private System.Windows.Forms.Button btnConnect;
	}
}