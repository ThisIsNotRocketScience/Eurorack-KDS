namespace Sim1
{
    partial class EdgeCutterTestFrame
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
            this.Attack = new System.Windows.Forms.TrackBar();
            this.label2 = new System.Windows.Forms.Label();
            this.Decay = new System.Windows.Forms.TrackBar();
            this.label3 = new System.Windows.Forms.Label();
            this.Sustain = new System.Windows.Forms.TrackBar();
            this.label4 = new System.Windows.Forms.Label();
            this.Release = new System.Windows.Forms.TrackBar();
            this.label5 = new System.Windows.Forms.Label();
            this.Curvature = new System.Windows.Forms.TrackBar();
            this.Mode = new System.Windows.Forms.GroupBox();
            this.radioButton3 = new System.Windows.Forms.RadioButton();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.radioButton5 = new System.Windows.Forms.RadioButton();
            this.radioButton4 = new System.Windows.Forms.RadioButton();
            this.button1 = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.DispUpdate = new System.Windows.Forms.Timer(this.components);
            this.EnvUpdate = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.flowLayoutPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Attack)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Decay)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Sustain)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Release)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.Curvature)).BeginInit();
            this.Mode.SuspendLayout();
            this.groupBox1.SuspendLayout();
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
            this.flowLayoutPanel1.Controls.Add(this.Attack);
            this.flowLayoutPanel1.Controls.Add(this.label2);
            this.flowLayoutPanel1.Controls.Add(this.Decay);
            this.flowLayoutPanel1.Controls.Add(this.label3);
            this.flowLayoutPanel1.Controls.Add(this.Sustain);
            this.flowLayoutPanel1.Controls.Add(this.label4);
            this.flowLayoutPanel1.Controls.Add(this.Release);
            this.flowLayoutPanel1.Controls.Add(this.label5);
            this.flowLayoutPanel1.Controls.Add(this.Curvature);
            this.flowLayoutPanel1.Controls.Add(this.Mode);
            this.flowLayoutPanel1.Controls.Add(this.groupBox1);
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
            this.label1.Size = new System.Drawing.Size(47, 17);
            this.label1.TabIndex = 1;
            this.label1.Text = "Attack";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // Attack
            // 
            this.Attack.AutoSize = false;
            this.Attack.Location = new System.Drawing.Point(3, 20);
            this.Attack.Maximum = 255;
            this.Attack.Name = "Attack";
            this.Attack.Size = new System.Drawing.Size(358, 25);
            this.Attack.TabIndex = 0;
            this.Attack.TickStyle = System.Windows.Forms.TickStyle.None;
            this.Attack.Value = 20;
            this.Attack.Scroll += new System.EventHandler(this.Attack_Scroll);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(3, 48);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(48, 17);
            this.label2.TabIndex = 9;
            this.label2.Text = "Decay";
            // 
            // Decay
            // 
            this.Decay.AutoSize = false;
            this.Decay.Location = new System.Drawing.Point(3, 68);
            this.Decay.Maximum = 255;
            this.Decay.Name = "Decay";
            this.Decay.Size = new System.Drawing.Size(358, 25);
            this.Decay.TabIndex = 1;
            this.Decay.TickStyle = System.Windows.Forms.TickStyle.None;
            this.Decay.Value = 30;
            this.Decay.Scroll += new System.EventHandler(this.Decay_Scroll);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(3, 96);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(55, 17);
            this.label3.TabIndex = 10;
            this.label3.Text = "Sustain";
            // 
            // Sustain
            // 
            this.Sustain.AutoSize = false;
            this.Sustain.Location = new System.Drawing.Point(3, 116);
            this.Sustain.Maximum = 255;
            this.Sustain.Name = "Sustain";
            this.Sustain.Size = new System.Drawing.Size(358, 25);
            this.Sustain.TabIndex = 2;
            this.Sustain.TickStyle = System.Windows.Forms.TickStyle.None;
            this.Sustain.Value = 50;
            this.Sustain.Scroll += new System.EventHandler(this.Sustain_Scroll);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(3, 144);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(60, 17);
            this.label4.TabIndex = 11;
            this.label4.Text = "Release";
            // 
            // Release
            // 
            this.Release.AutoSize = false;
            this.Release.Location = new System.Drawing.Point(3, 164);
            this.Release.Maximum = 255;
            this.Release.Name = "Release";
            this.Release.Size = new System.Drawing.Size(358, 25);
            this.Release.TabIndex = 3;
            this.Release.TickStyle = System.Windows.Forms.TickStyle.None;
            this.Release.Value = 60;
            this.Release.Scroll += new System.EventHandler(this.Release_Scroll);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(3, 192);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(70, 17);
            this.label5.TabIndex = 12;
            this.label5.Text = "Curvature";
            // 
            // Curvature
            // 
            this.Curvature.AutoSize = false;
            this.Curvature.Location = new System.Drawing.Point(3, 212);
            this.Curvature.Maximum = 255;
            this.Curvature.Name = "Curvature";
            this.Curvature.Size = new System.Drawing.Size(358, 25);
            this.Curvature.TabIndex = 4;
            this.Curvature.TickStyle = System.Windows.Forms.TickStyle.None;
            this.Curvature.Value = 50;
            this.Curvature.Scroll += new System.EventHandler(this.Curvature_Scroll);
            // 
            // Mode
            // 
            this.Mode.AutoSize = true;
            this.Mode.Controls.Add(this.radioButton3);
            this.Mode.Controls.Add(this.radioButton2);
            this.Mode.Controls.Add(this.radioButton1);
            this.Mode.Location = new System.Drawing.Point(3, 243);
            this.Mode.Name = "Mode";
            this.Mode.Size = new System.Drawing.Size(99, 117);
            this.Mode.TabIndex = 5;
            this.Mode.TabStop = false;
            this.Mode.Text = "Mode";
            // 
            // radioButton3
            // 
            this.radioButton3.AutoSize = true;
            this.radioButton3.Location = new System.Drawing.Point(18, 75);
            this.radioButton3.Name = "radioButton3";
            this.radioButton3.Size = new System.Drawing.Size(61, 21);
            this.radioButton3.TabIndex = 3;
            this.radioButton3.Text = "Loop";
            this.radioButton3.UseVisualStyleBackColor = true;
            this.radioButton3.CheckedChanged += new System.EventHandler(this.radioButton3_CheckedChanged);
            // 
            // radioButton2
            // 
            this.radioButton2.AutoSize = true;
            this.radioButton2.Location = new System.Drawing.Point(18, 48);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(75, 21);
            this.radioButton2.TabIndex = 2;
            this.radioButton2.Text = "Trigger";
            this.radioButton2.UseVisualStyleBackColor = true;
            this.radioButton2.CheckedChanged += new System.EventHandler(this.radioButton2_CheckedChanged);
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.Checked = true;
            this.radioButton1.Location = new System.Drawing.Point(18, 21);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(60, 21);
            this.radioButton1.TabIndex = 1;
            this.radioButton1.TabStop = true;
            this.radioButton1.Text = "Gate";
            this.radioButton1.UseVisualStyleBackColor = true;
            this.radioButton1.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.AutoSize = true;
            this.groupBox1.Controls.Add(this.radioButton5);
            this.groupBox1.Controls.Add(this.radioButton4);
            this.groupBox1.Location = new System.Drawing.Point(3, 366);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(82, 90);
            this.groupBox1.TabIndex = 7;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Speed";
            // 
            // radioButton5
            // 
            this.radioButton5.AutoSize = true;
            this.radioButton5.Location = new System.Drawing.Point(18, 48);
            this.radioButton5.Name = "radioButton5";
            this.radioButton5.Size = new System.Drawing.Size(58, 21);
            this.radioButton5.TabIndex = 2;
            this.radioButton5.Text = "Slow";
            this.radioButton5.UseVisualStyleBackColor = true;
            this.radioButton5.CheckedChanged += new System.EventHandler(this.radioButton5_CheckedChanged);
            // 
            // radioButton4
            // 
            this.radioButton4.AutoSize = true;
            this.radioButton4.Checked = true;
            this.radioButton4.Location = new System.Drawing.Point(18, 21);
            this.radioButton4.Name = "radioButton4";
            this.radioButton4.Size = new System.Drawing.Size(56, 21);
            this.radioButton4.TabIndex = 1;
            this.radioButton4.TabStop = true;
            this.radioButton4.Text = "Fast";
            this.radioButton4.UseVisualStyleBackColor = true;
            this.radioButton4.CheckedChanged += new System.EventHandler(this.radioButton4_CheckedChanged);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(3, 462);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(179, 39);
            this.button1.TabIndex = 8;
            this.button1.Text = "Trigger";
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
            // EdgeCutterTestFrame
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(763, 635);
            this.Controls.Add(this.splitContainer1);
            this.Name = "EdgeCutterTestFrame";
            this.Text = "TINRS EdgeCutter Test Framework";
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.flowLayoutPanel1.ResumeLayout(false);
            this.flowLayoutPanel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Attack)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Decay)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Sustain)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Release)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.Curvature)).EndInit();
            this.Mode.ResumeLayout(false);
            this.Mode.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.FlowLayoutPanel flowLayoutPanel1;
        private System.Windows.Forms.TrackBar Attack;
        private System.Windows.Forms.TrackBar Decay;
        private System.Windows.Forms.TrackBar Sustain;
        private System.Windows.Forms.TrackBar Release;
        private System.Windows.Forms.TrackBar Curvature;
        private System.Windows.Forms.GroupBox Mode;
        private System.Windows.Forms.RadioButton radioButton3;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.RadioButton radioButton5;
        private System.Windows.Forms.RadioButton radioButton4;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Timer DispUpdate;
        private System.Windows.Forms.Timer EnvUpdate;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
    }
}

