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
    public partial class BigFishTestFrame :  WeifenLuo.WinFormsUI.Docking.DockContent
    {
        public BigFishTestFrame()
        {
            InitializeComponent();
        }

        private void pictureBox1_Resize(object sender, EventArgs e)
        {
            pictureBox1.Invalidate();
        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            var G = e.Graphics;

            G.Clear(Color.Black);

            int x = 2400;
            int y = 3600;
            int r = 400;

            for (int i =0;i< 7;i++)
            {
                int xx = x + (int)(Math.Sin((i+1) * (Math.PI*2) / 8.0)*r);
                int yy = y + (int)(Math.Cos((i+1) * (Math.PI*2) / 8.0) * r);
                G.FillRectangle(Brushes.Yellow, new RectangleF((xx-5) / 10, (yy-5) / 10, 10, 10));

                G.DrawString(String.Format("{0},{1}", xx, yy), new Font("Arial", 10), Brushes.AliceBlue, new Point(x / 10, y / 10 + 40 + 13 * i));
            }

            x = 3465;
            y = 1650;
            r = x - 3181;

            for (int i = 0; i < 5; i++)
            {
                int xx = x + (int)(Math.Sin((i +6 ) * (Math.PI * 2) / 8.0) * r);
                int yy = y + (int)(Math.Cos((i +6) * (Math.PI * 2) / 8.0) * r);
                G.FillRectangle(Brushes.Yellow, new RectangleF((xx - 5) / 10, (yy - 5) / 10, 10, 10));

                G.DrawString(String.Format("{0},{1}", xx, yy), new Font("Arial", 10), Brushes.AliceBlue, new Point(x / 10, y / 10 + 40 + 13 * i));
            }
        }
    }
}
