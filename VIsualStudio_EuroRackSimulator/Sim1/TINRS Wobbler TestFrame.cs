using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Sim1
{
    public partial class WobblerTestFrame :  WeifenLuo.WinFormsUI.Docking.DockContent
    {
        public WobblerTestFrame()
        {
            for (int i = 0; i < 4000; i++)
            {
                values.Add(0);
                linvalues.Add(0);
                values2.Add(0);
                linvalues2.Add(0);
            }

            TestFrameLoader.Init();
            InitializeComponent();
            
            RebuildLFO();
        }


        private void Attack_Scroll(object sender, EventArgs e)
        {
            RebuildLFO();
            pictureBox1.Invalidate();
        }

        private void Decay_Scroll(object sender, EventArgs e)
        {
            RebuildLFO();

            pictureBox1.Invalidate();
        }

        private void Sustain_Scroll(object sender, EventArgs e)
        {
            RebuildLFO();
            pictureBox1.Invalidate();
        }

        private void Release_Scroll(object sender, EventArgs e)
        {
            RebuildLFO();        
            pictureBox1.Invalidate();
        }

        private void RebuildLFO()
        {
            TestFrameLoader.ResetStatic();
            for (int i = 0; i < Math.Min(4000, pictureBox1.Width); i++)
            {
                if (i == 10)
                {
                    TestFrameLoader.LFOTrigger(1, 1);
                }
                if (i == 210)
                {
                    TestFrameLoader.LFOTrigger(0, 1);
                }

                double D = 0;
                
                values2[i] = TestFrameLoader.GetLFO(1, Speed.Value, Shape.Value, Mod.Value, Phase.Value) / 4096.0f;
                D = TestFrameLoader.GetLFOPhased(1) / 4096.0f;
                linvalues2[i] = D;
            }
        }

        private void Curvature_Scroll(object sender, EventArgs e)
        {
            RebuildLFO();
            pictureBox1.Invalidate();
        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            var g = e.Graphics;
            g.Clear(Color.Black);
            g.SmoothingMode = SmoothingMode.AntiAlias;
            g.InterpolationMode = InterpolationMode.HighQualityBicubic;
            g.CompositingQuality = CompositingQuality.HighQuality;

            List<PointF> P = new List<PointF>(pictureBox1.Width);
            List<PointF> P2 = new List<PointF>(pictureBox1.Width);
            List<PointF> P3 = new List<PointF>(pictureBox1.Width);
            List<PointF> P4 = new List<PointF>(pictureBox1.Width);
            for (int i = 0; i < pictureBox1.Width; i++)
            {
                int p1 = (i + pos + 4000 - pictureBox1.Width) % 4000;
                P.Add(new PointF(i, ScaleVal(values[p1])));
                P2.Add(new PointF(i, ScaleVal(linvalues[p1])));
                P3.Add(new PointF(i, (pictureBox1.Height / 2) + ScaleVal(values2[i])));
                P4.Add(new PointF(i, (pictureBox1.Height / 2) + ScaleVal(linvalues2[i])));
            }

            g.DrawLines(new Pen(Color.Green, 2.0f), P2.ToArray());
            g.DrawLines(new Pen(Color.Yellow, 2.0f), P.ToArray());
            g.DrawLines(new Pen(Color.Green, 2.0f), P4.ToArray());
            g.DrawLines(new Pen(Color.Yellow, 2.0f), P3.ToArray());

            for (int i = 0; i < 2; i++)
            {
                int B = TestFrameLoader.GetLFOGate(i);
                if (B > 0) B = 255;
                var C = Color.FromArgb(B, B, 0);
                Rectangle R = new Rectangle();
                R.Width = 10;
                R.Height = 10;
                R.X = pictureBox1.Width - 12 - i * 12;
                R.Y = 2;
                g.FillRectangle(new SolidBrush(C), R);
                g.DrawRectangle(new Pen(Color.White, 1), R);

            }

            for (int i = 0; i < 12; i++)
            {
                int B = TestFrameLoader.GetLFOLed(i);
                var C = Color.FromArgb(B, B, 0);
                Rectangle R = new Rectangle();
                R.Width = 10;
                R.Height = 10;
                R.X = 2 + i * 12;
                R.Y = 2;
                g.FillRectangle(new SolidBrush(C), R);
                g.DrawRectangle(new Pen(Color.White, 1), R);
            }
            
        }

        float ScaleVal(double inp)
        {
            return 20 + (float)((pictureBox1.Height / 2 - 40) * (1.0 - inp));
        }

        List<double> values = new List<double>(4000);
        List<double> linvalues = new List<double>(4000);

        List<double> values2 = new List<double>(4000);
        List<double> linvalues2 = new List<double>(4000);
        int pos = 0;

        private void pictureBox1_Resize(object sender, EventArgs e)
        {
            RebuildLFO();
            pictureBox1.Invalidate();
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }



        private void EnvUpdate_Tick(object sender, EventArgs e)
        {
            for (int i = 0; i < 10; i++)
            {
                pos = (pos + 1) % 4000;
                double D = 0;
                
                values[pos] = TestFrameLoader.GetLFO(0, Speed.Value, Shape.Value, Mod.Value, Phase.Value) / 4096.0f;
                D = TestFrameLoader.GetLFOPhased(0)/ 4096.0f;

                linvalues[pos] = D;
            }
        }


        private void DispUpdate_Tick(object sender, EventArgs e)
        {
            pictureBox1.Invalidate();
        }

        private void button1_MouseDown(object sender, MouseEventArgs e)
        {
            TestFrameLoader.LFOTrigger(1, 0);
        }

        private void button1_MouseUp(object sender, MouseEventArgs e)
        {
            TestFrameLoader.LFOTrigger(0, 0);

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }


}
