using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Sim1
{
    public partial class TINRS_Tuesday_TestFrame : WeifenLuo.WinFormsUI.Docking.DockContent
    {
        TestSynth TS = new TestSynth();

        public TINRS_Tuesday_TestFrame()
        {
            TestFrameLoader.Init();
            TS.Open();
            TS.Play();
            TS.Set(440.0, 0);
            InitializeComponent();
            UpdatePattern();
        }

        private void TINRS_Tuesday_TestFrame_FormClosing(object sender, FormClosingEventArgs e)
        {
            TS.Stop();
        }

        private void TINRS_Tuesday_TestFrame_Load(object sender, EventArgs e)
        {

        }

        int msec = 0;
        int CVOut = 0;
        int VelOut = 0;
        bool Gate = false;
        bool Accent = false;

        private void timer1_Tick(object sender, EventArgs e)
        {
            msec += 10;

            int BPM = (int)BPMupdown.Value;
            int TickMsec = 60000 / (96*BPM/4);
            
            if (Playing)
            {
                while (msec > TickMsec)
                {
                    msec -= TickMsec;

                    TestFrameLoader.Tuesday_ClockTick(1, 1);
                    TestFrameLoader.Tuesday_ClockTick(1, 0);
                    for(int i =0;i< msec*2;i++) TestFrameLoader.Tuesday_DoTimer();

                    Gate = TestFrameLoader.Tuesday_GetGate();
                    Accent = TestFrameLoader.Tuesday_GetAccent();
                    CVOut = TestFrameLoader.Tuesday_GetCV();
                    VelOut = TestFrameLoader.Tuesday_GetVel();


                    //#define DAC_VOLT(x) ((int)((4096.0 * (x)) / (2.5 * 2.048)))
                    //#define DAC_NOTE(x) DAC_VOLT((x) / 12.0)
                    double volt = (CVOut / 4096.0) * (2.5 * 2.048);
                    double note = volt * 12.0;
                    TS.Set(440 * Math.Pow(2, note / 12), Gate ? 0.2 : 0);
                    
                }
            }
        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            var G = e.Graphics;

            G.Clear(Color.Black);
            int octs = 4;

            G.TranslateTransform(0, pictureBox1.Height);
            G.ScaleTransform(1, -1);
            float BarH = (pictureBox1.Height - 4) / (octs * 12.0f);
            Brush Black = new SolidBrush(Color.FromArgb(40, 40, 40));
            Brush White = new SolidBrush(Color.FromArgb(200, 200, 200));
            RectangleF R = new RectangleF();
            R.Width = pictureBox1.Width - 4;
            R.Height = BarH - 2;
            R.X = 2;

            for (int i = 0; i < octs * 12; i++)
            {
                R.Y = i * BarH + 2;
                Brush B = Black;
                switch (i % 12)
                {
                    case 0: B = White; break;
                    case 2: B = White; break;
                    case 4: B = White; break;
                    case 5: B = White; break;
                    case 7: B = White; break;
                    case 9: B = White; break;
                    case 11: B = White; break;
                }
                G.FillRectangle(B, R);
            }

            float TickW = (pictureBox1.Width - 4) /(float)Math.Max(1, (Pattern.Count));
            R.Height--;
            R.Width = TickW - 2;
           
            for(int i =0;i<Pattern.Count;i++)
            {
                G.DrawLine(new Pen(Color.FromArgb(50, 0, 0, 0)), i * TickW, 0, i * TickW, pictureBox1.Height);
                R.X = i * TickW+2;
                R.Y = Pattern[i].note * BarH + 2;
                Brush Box = new SolidBrush(Color.FromArgb(230, Pattern[i].vel, 50));
                G.FillRectangle(Box, R);
            }
        }

        class Tick
        {
            public int note;
            public bool accent;
            public bool gate;
            public int vel;
        }

        List<Tick> Pattern = new List<Tick>();

        bool Playing = false;

        void UpdatePattern()
        {
            int Algo = 0 ;
            int Scale = 0;
            int Beats = 0;
            int Ticks = 0;

            if (radioButton1.Checked) Algo = 0;
            if (radioButton4.Checked) Algo = 1;
            if (radioButton3.Checked) Algo = 2;
            if (radioButton2.Checked) Algo = 3;

            if (radioButton5.Checked) Scale = 0;
            if (radioButton6.Checked) Scale = 1;
            if (radioButton7.Checked) Scale = 2;
            if (radioButton8.Checked) Scale = 3;

            if (radioButton9.Checked) Ticks = 0;
            if (radioButton10.Checked) Ticks = 1;
            if (radioButton11.Checked) Ticks = 2;
            if (radioButton12.Checked) Ticks = 3;

            if (radioButton13.Checked) Beats = 0;
            if (radioButton14.Checked) Beats = 1;
            if (radioButton15.Checked) Beats = 2;
            if (radioButton16.Checked) Beats = 3;

            Pattern.Clear();
            TestFrameLoader.Tuesday_UpdatePattern(Algo, Scale,Ticks, Beats,(int)TempoSlider.Value,  (int)XSlider.Value, (int)YSlider.Value, (int)ISlider.Value);
            int PTicks = TestFrameLoader.Tuesday_GetPatternLength();
            for(int i =0;i<PTicks;i++)
            {
                Tick T = new Tick();
                T.note = TestFrameLoader.Tuesday_GetTickNote(i);
                T.vel = TestFrameLoader.Tuesday_GetTickVel(i);
                T.accent = TestFrameLoader.Tuesday_GetTickAccent(i)>0;
                T.gate = TestFrameLoader.Tuesday_GetTickGate(i)>0;

                Pattern.Add(T);
            }

            pictureBox1.Invalidate();
        }

        private void pictureBox1_Resize(object sender, EventArgs e)
        {
            pictureBox1.Invalidate();
        }

        private void XSlider_Scroll(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void YSlider_Scroll(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void ISlider_Scroll(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void radioButton7_CheckedChanged(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void radioButton5_CheckedChanged(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void radioButton6_CheckedChanged(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void radioButton8_CheckedChanged(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {            
            UpdatePattern();
        }

        private void radioButton4_CheckedChanged(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void radioButton11_CheckedChanged(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void radioButton9_CheckedChanged(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void radioButton10_CheckedChanged(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void radioButton12_CheckedChanged(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void radioButton15_CheckedChanged(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void radioButton13_CheckedChanged(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void radioButton14_CheckedChanged(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void radioButton16_CheckedChanged(object sender, EventArgs e)
        {
            UpdatePattern();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Playing = true;

        }

        private void button3_Click(object sender, EventArgs e)
        {
            Playing = false;
        }

        private void splitContainer2_Panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void splitContainer1_SplitterMoved(object sender, SplitterEventArgs e)
        {

        }
    }
}
