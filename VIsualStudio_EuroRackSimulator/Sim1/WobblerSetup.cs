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
    public partial class WobblerSetup : WeifenLuo.WinFormsUI.Docking.DockContent
    {
        public WobblerSetup()
        {
            InitializeComponent();
            TestFrameLoader.Init();
            
        }

        private void pictureBox1_Resize(object sender, EventArgs e)
        {
            pictureBox1.Invalidate();
        }


        class PendulumDisp
        {
            public float[] F1Hist = new float[300];
            public float[] F2Hist = new float[300];
            public PointF[] P1 = new PointF[300];
            public PointF[] P2 = new PointF[300];
            public void Render(Graphics G, int x, int y )
            {
                {


                    float xx1 = 120 + x;
                    float xy1 = 120 + y;
                    float xx2 = xx1 + (float)Math.Sin(F1Hist[299]) * 100 ;
                    float xy2 = xy1 + (float)Math.Cos(F1Hist[299]) * 100 ;
                    float xx3 = xx2 + (float)Math.Sin(F2Hist[299]) * 100 ;
                    float xy3 = xy2 + (float)Math.Cos(F2Hist[299]) * 100 ;

                    G.DrawLine(Pens.White, xx1, xy1, xx2, xy2);
                    G.DrawLine(Pens.White, xx2, xy2, xx3, xy3);


                    for (int i = 0; i < 300; i++)
                    {
                        P1[i] = new PointF(i*2  + x, y+ Math.Max(0, Math.Min(1000, 100 + F1Hist[i]*30)));
                        P2[i] = new PointF(i*2  + x, y+ Math.Max(0, Math.Min(1000, 200 + F2Hist[i]*30)));
                    }

                    G.DrawLines(Pens.Yellow, P1);
                    G.DrawLines(Pens.Blue, P2);
                }

            }

            public void Update(float p1, float p2)
            {
                for (int i = 0; i < 299; i++)
                {
                    F1Hist[i] = F1Hist[i + 1];
                    F2Hist[i] = F2Hist[i + 1];
                }

                F1Hist[299] = (p1);
                F2Hist[299] = (p2);

            }
        }


        PendulumDisp IntDisp = new PendulumDisp();
        PendulumDisp FloatDisp = new PendulumDisp();

        class Mass
        {
           public PointF Pos;
            public PointF Speed;
            public PointF Force;
        }

        class Spring
        {
            public Mass A;
            public Mass B;
            public float RestLength;
        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {

            var G = e.Graphics;
            G.Clear(Color.Black);
            G.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
            G.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;


            for (int i = 0; i < 5; i++)
            {
                TestFrameLoader.RunPendulum();
                TestFrameLoader.RunPendulumInt();

                //TestFrameLoader.Compare();
            }

            float F1 = TestFrameLoader.RunPendulum();
            float F2 = TestFrameLoader.RunPendulum2();

            float iF1 = (float)TestFrameLoader.RunPendulumInt();
            float iF2 = (float)TestFrameLoader.RunPendulum2Int();

            // F1 *= 6.283f  / ((float)(1 <<24)) ;
            // F2 *= 6.283f / ((float)(1 << 24));

            F1 /= (float)0xffff;
            F2 /= (float)0xffff;
            iF1 *= 1.0f / (float)(1 << 25);
            iF2 *= 1.0f / (float)(1 << 25);

          

            FloatDisp.Update(F1, F2);
            IntDisp.Update(iF1, iF2);

            FloatDisp.Render(G, 10, 0);
            IntDisp.Render(G, 620, 0);
            
            float S = (float)Math.Min(pictureBox1.Width / ((10 + 5.08 * 12)), pictureBox1.Height / (10 + 128.5));
            //S = 1.0f / S;
            float S2 = 1.0f / S;
            G.ScaleTransform(S, S);
            G.TranslateTransform(5, 5);
            float H = 128.5f;
            float Hi = 106.5f;
            float W = (5.08f * 12);
            float R = 1.5f;
            G.DrawRectangle(new Pen(Color.Gray, S2), 0, (H - Hi) / 2, W, Hi);
            G.DrawRectangle(new Pen(Color.White, S2), 0, 0, W, H);
            
            float x1 = -6;
            float x2 = (W - x1);
            float W1 = W/2 - R * 2 ;
            float y1 = H - 2.54f - 11;
        //    string RR = "";
          //  List<string> dnames = new List<string>{ "d10", "d11", "d33", "d4", "d5", "d6", "d7", "d8", "d9" };
            for (int i =0;i<9;i++)
            {
                float p = (i * 3.1415f) / 19.0f - 0.1f;
                float cx = -(float)Math.Sin(p) * W1;
                float sx = (float)Math.Cos(p) * W1 ;

                int lA = TestFrameLoader.GetWobbleLed(i, 0);
                int lB = TestFrameLoader.GetWobbleLed(i, 1);
                G.FillEllipse(new SolidBrush(Color.FromArgb(lA,0,0)), x1 + sx - R, y1 + cx - R, R * 2, R * 2);
                G.FillEllipse(new SolidBrush(Color.FromArgb(lB,0,0)), W - (x1 + sx) - R, y1 + cx - R, R * 2, R * 2);


                G.DrawEllipse(new Pen(Color.Yellow, S2), x1 + sx - R, y1 + cx - R, R*2, R*2);
                G.DrawEllipse(new Pen(Color.Yellow, S2), W - (x1 + sx ) - R, y1 + cx - R, R*2, R*2);
//                RR += String.Format("MOVE D{0} ({1} {2}); ", i + 24, (x1 + sx).ToString().Replace(',','.'), (H*2 - ( y1 + cx)).ToString().Replace(',', '.'));
  //              RR += String.Format("MOVE {0} ({1} {2}); ", dnames[i], (W - (x1 + sx)).ToString().Replace(',', '.'), (H * 2 - (y1 + cx)).ToString().Replace(',', '.'));
            }
       //     Console.WriteLine(RR);
            //MessageBox.Show(RR);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            pictureBox1.Invalidate();
        }

        private void TriggerButton_Click(object sender, EventArgs e)
        {
            TestFrameLoader.TriggerWobbler();
        }
    }
}
