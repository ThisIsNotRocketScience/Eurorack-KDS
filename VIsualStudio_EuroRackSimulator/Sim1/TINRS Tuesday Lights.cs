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
    public partial class TINRS_Tuesday_Lights: WeifenLuo.WinFormsUI.Docking.DockContent
    {
    
        public TINRS_Tuesday_Lights()
        {
            InitializeComponent();
        }

        public void DrawSquare(Graphics G, int x, int y,bool filled)
        {
            if (filled)
            {
                G.FillRectangle(new SolidBrush(Color.Yellow), x, y, minisquare, minisquare);
            }
            G.DrawRectangle(new Pen(Color.White, 1.0f), x, y, minisquare, minisquare);
        }


        public void Draw4Square(Graphics G, int x, int y, int num)
        {
            DrawSquare(G, x, y, num == 0);
            DrawSquare(G, x+ minisquare, y, num == 1);
            DrawSquare(G, x+ minisquare, y+ minisquare, num == 2);
            DrawSquare(G, x, y+ minisquare, num == 3);
        }
        int margin = 5;
        const int minisquare = 10;
        int squaresize = minisquare*2;

        public void DrawSquares(Graphics G, int x, int y, int num, string label)
        {
            Draw4Square(G, x+squaresize + margin, y, num&3);
            Draw4Square(G, x+squaresize+margin, y+squaresize + margin, (num>>2)&3);
            Draw4Square(G, x, y+squaresize+margin, (num>>4)&3);
            G.DrawString(label, new Font("Panton ExtraBold", 15), new SolidBrush(Color.White), x + squaresize * 2 + margin * 2, y + squaresize - 7.5f);
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.Clear(Color.Black);
            Graphics G = e.Graphics;
            for(int i =0;i<16;i++)
            {
                DrawSquares(G, (i%5)*300 + 10 , 10 + (i/5) * (squaresize *2 + margin*6), i, "Some thing");
            }
        }

        private void pictureBox1_Resize(object sender, EventArgs e)
        {
            pictureBox1.Invalidate();
        }
    }
}
