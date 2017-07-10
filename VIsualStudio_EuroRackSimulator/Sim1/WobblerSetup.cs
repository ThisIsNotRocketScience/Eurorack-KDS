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

        float[] F1Hist = new float[300];
        float[] F2Hist = new float[300];
        PointF[] P1 = new PointF[300];
        PointF[] P2 = new PointF[300];

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {

            float F1 = TestFrameLoader.RunPendulumInt();
            float F2 = TestFrameLoader.RunPendulum2Int();
            for(int i =0;i<299;i++)
            {
                F1Hist[i] = F1Hist[i + 1];
                F2Hist[i] = F2Hist[i + 1];
            }
            F1Hist[299] =  (F1/(float)(0xffff)) % (3.1415f*2);
            F2Hist[299] = (F2 / (float)(0xffff))% (3.1415f*2);
            e.Graphics.Clear(Color.Black);

            var G = e.Graphics;
            G.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
            G.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.HighQuality;

            {
                float xx1 = 100;
                float xy1 = 100;
                float xx2 = xx1 + (float)Math.Sin(F1/ (float)(0xffff)) * 100;
                float xy2 = xy1 + (float)Math.Cos(F1/ (float)(0xffff)) * 100;
                float xx3 = xx2 + (float)Math.Sin(F2/ (float)(0xffff)) * 100;
                float xy3 = xy2 + (float)Math.Cos(F2/ (float)(0xffff)) * 100;

                G.DrawLine(Pens.White, xx1, xy1, xx2, xy2);
                G.DrawLine(Pens.White, xx2, xy2, xx3, xy3);

                for(int i =0;i<300;i++)
                {
                    P1[i] = new PointF(i*2, 200 + F1Hist[i] * 30);
                    P2[i] = new PointF(i*2, 400 + F2Hist[i] * 30);
                }

                G.DrawLines(Pens.Yellow, P1);
                G.DrawLines(Pens.Blue, P2);
            }

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
    }
}
