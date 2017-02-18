using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using WeifenLuo.WinFormsUI.Docking;
using WeifenLuo.WinFormsUI.ThemeVS2015;


namespace Sim1
{
    public partial class MainForm : Form
    {
        private DockPanel dockPanel;


        public MainForm()
        {
            InitializeComponent();
            this.dockPanel = new WeifenLuo.WinFormsUI.Docking.DockPanel();

            var theme = new VS2015DarkTheme();
            this.dockPanel.Theme = theme;

            this.dockPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Controls.Add(this.dockPanel);

            dockPanel.UpdateDockWindowZOrder(DockStyle.Left, true);

            TINRS_Tuesday_Lights lf = new TINRS_Tuesday_Lights();
            lf.Show(this.dockPanel, DockState.Document);
            lf.Text = "Tuesday Lights";


            TINRS_Tuesday_TestFrame tf = new TINRS_Tuesday_TestFrame();
            tf.Show(this.dockPanel, DockState.Document);
            tf.Text = "Tuesday";




            WobblerTestFrame wf = new WobblerTestFrame();
            wf.Show(this.dockPanel, DockState.Document);
            wf.Text = "Wobbler";


            EdgeCutterTestFrame ef = new EdgeCutterTestFrame();
            ef.Show(this.dockPanel, DockState.Document);
            ef.Text = "EdgeCutter";

            

        }
    }
}
