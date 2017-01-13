namespace Sim1
{
    partial class WobblerTestFrame
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
            this.components = new System.ComponentModel.Container();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.flowLayoutPanel1 = new System.Windows.Forms.FlowLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.Speed = new System.Windows.Forms.TrackBar();
            this.label2 = new System.Windows.Forms.Label();
            this.Shape = new System.Windows.Forms.TrackBar();
            this.label3 = new System.Windows.Forms.Label();
            this.Mod = new System.Windows.Forms.TrackBar();
            this.label4 = new System.Windows.Forms.Label();
            this.Phase = new System.Windows.Forms.TrackBar();
            this.button1 = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.DispUpdate = new System.Windows.Forms.Timer(this.components);
            this.EnvUpdate = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.flowLayoutPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Speed)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Shape)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Mod)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Phase)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.flowLayoutPanel1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.pictureBox1);
            this.splitContainer1.Size = new System.Drawing.Size(763, 635);
            this.splitContainer1.SplitterDistance = 412;
            this.splitContainer1.TabIndex = 0;
            // 
            // flowLayoutPanel1
            // 
            this.flowLayoutPanel1.Controls.Add(this.label1);
            this.flowLayoutPanel1.Controls.Add(this.Speed);
            this.flowLayoutPanel1.Controls.Add(this.label2);
            this.flowLayoutPanel1.Controls.Add(this.Shape);
            this.flowLayoutPanel1.Controls.Add(this.label3);
            this.flowLayoutPanel1.Controls.Add(this.Mod);
            this.flowLayoutPanel1.Controls.Add(this.label4);
            this.flowLayoutPanel1.Controls.Add(this.Phase);
            this.flowLayoutPanel1.Controls.Add(this.button1);
            this.flowLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.flowLayoutPanel1.FlowDirection = System.Windows.Forms.FlowDirection.TopDown;
            this.flowLayoutPanel1.Location = new System.Drawing.Point(0, 0);
            this.flowLayoutPanel1.Name = "flowLayoutPanel1";
            this.flowLayoutPanel1.Size = new System.Drawing.Size(412, 635);
            this.flowLayoutPanel1.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(3, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(49, 17);
            this.label1.TabIndex = 1;
            this.label1.Text = "Speed";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // Speed
            // 
            this.Speed.AutoSize = false;
            this.Speed.Location = new System.Drawing.Point(3, 20);
            this.Speed.Maximum = 255;
            this.Speed.Name = "Speed";
            this.Speed.Size = new System.Drawing.Size(358, 25);
            this.Speed.TabIndex = 0;
            this.Speed.TickStyle = System.Windows.Forms.TickStyle.None;
            this.Speed.Value = 220;
            this.Speed.Scroll += new System.EventHandler(this.Attack_Scroll);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 48);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(49, 17);
            this.label2.TabIndex = 9;
            this.label2.Text = "Shape";
            // 
            // Shape
            // 
            this.Shape.AutoSize = false;
            this.Shape.Location = new System.Drawing.Point(3, 68);
            this.Shape.Maximum = 255;
            this.Shape.Name = "Shape";
            this.Shape.Size = new System.Drawing.Size(358, 25);
            this.Shape.TabIndex = 1;
            this.Shape.TickStyle = System.Windows.Forms.TickStyle.None;
            this.Shape.Value = 30;
            this.Shape.Scroll += new System.EventHandler(this.Decay_Scroll);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 96);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(35, 17);
            this.label3.TabIndex = 10;
            this.label3.Text = "Mod";
            // 
            // Mod
            // 
            this.Mod.AutoSize = false;
            this.Mod.Location = new System.Drawing.Point(3, 116);
            this.Mod.Maximum = 255;
            this.Mod.Name = "Mod";
            this.Mod.Size = new System.Drawing.Size(358, 25);
            this.Mod.TabIndex = 2;
            this.Mod.TickStyle = System.Windows.Forms.TickStyle.None;
            this.Mod.Value = 50;
            this.Mod.Scroll += new System.EventHandler(this.Sustain_Scroll);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 144);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(48, 17);
            this.label4.TabIndex = 11;
            this.label4.Text = "Phase";
            // 
            // Phase
            // 
            this.Phase.AutoSize = false;
            this.Phase.Location = new System.Drawing.Point(3, 164);
            this.Phase.Maximum = 255;
            this.Phase.Name = "Phase";
            this.Phase.Size = new System.Drawing.Size(358, 25);
            this.Phase.TabIndex = 3;
            this.Phase.TickStyle = System.Windows.Forms.TickStyle.None;
            this.Phase.Value = 60;
            this.Phase.Scroll += new System.EventHandler(this.Release_Scroll);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(10, 202);
            this.button1.Margin = new System.Windows.Forms.Padding(10);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(325, 39);
            this.button1.TabIndex = 8;
            this.button1.Text = "Reset";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            this.button1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.button1_MouseDown);
            this.button1.MouseUp += new System.Windows.Forms.MouseEventHandler(this.button1_MouseUp);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBox1.Location = new System.Drawing.Point(0, 0);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(347, 635);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.Paint += new System.Windows.Forms.PaintEventHandler(this.pictureBox1_Paint);
            this.pictureBox1.Resize += new System.EventHandler(this.pictureBox1_Resize);
            // 
            // DispUpdate
            // 
            this.DispUpdate.Enabled = true;
            this.DispUpdate.Interval = 20;
            this.DispUpdate.Tick += new System.EventHandler(this.DispUpdate_Tick);
            // 
            // EnvUpdate
            // 
            this.EnvUpdate.Enabled = true;
            this.EnvUpdate.Interval = 10;
            this.EnvUpdate.Tick += new System.EventHandler(this.EnvUpdate_Tick);
            // 
            // WobblerTestFrame
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(763, 635);
            this.CloseButton = false;
            this.CloseButtonVisible = false;
            this.Controls.Add(this.splitContainer1);
            this.Name = "WobblerTestFrame";
            this.Text = "TINRS EdgeCutter Test Framework";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.flowLayoutPanel1.ResumeLayout(false);
            this.flowLayoutPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Speed)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Shape)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Mod)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Phase)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.TrackBar Speed;
        private System.Windows.Forms.TrackBar Shape;
        private System.Windows.Forms.TrackBar Mod;
        private System.Windows.Forms.TrackBar Phase;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Timer DispUpdate;
        private System.Windows.Forms.Timer EnvUpdate;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
    }
}

