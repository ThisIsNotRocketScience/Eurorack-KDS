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
    public partial class EdgeCutterTestFrame :  WeifenLuo.WinFormsUI.Docking.DockContent
    {
        public EdgeCutterTestFrame()
        {
            TestFrameLoader.Init();
            for (int i = 0; i < 4000; i++)
            {
                values.Add(0);
                linvalues.Add(0);
                values2.Add(0);
                linvalues2.Add(0);
                triggers.Add(false);
                triggers2.Add(false);
            }

            InitializeComponent();
           
            RebuildEnv();
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            UpdateModeCheck();
            pictureBox1.Invalidate();
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            UpdateModeCheck();
            pictureBox1.Invalidate();
        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            UpdateModeCheck();
            pictureBox1.Invalidate();
        }

        private void UpdateModeCheck()
        {
            RebuildEnv();

            if (radioButton1.Checked) EnvMode = Envelope.EnvelopeMode.Gate;
            if (radioButton2.Checked) EnvMode = Envelope.EnvelopeMode.Trigger;
            if (radioButton3.Checked) EnvMode = Envelope.EnvelopeMode.Loop;
        }

        private void radioButton4_CheckedChanged(object sender, EventArgs e)
        {
            UpdateSpeedCheck();
            pictureBox1.Invalidate();
        }

        private void radioButton5_CheckedChanged(object sender, EventArgs e)
        {
            UpdateSpeedCheck();
            pictureBox1.Invalidate();
        }

        private void UpdateSpeedCheck()
        {
            RebuildEnv();

            if (radioButton4.Checked) TimeScale = Envelope.EnvelopeTimeScale.Short;
            if (radioButton5.Checked) TimeScale = Envelope.EnvelopeTimeScale.Long;

        }

        private void Attack_Scroll(object sender, EventArgs e)
        {
            RebuildEnv();
            pictureBox1.Invalidate();
        }

        private void Decay_Scroll(object sender, EventArgs e)
        {
            RebuildEnv();

            pictureBox1.Invalidate();
        }

        private void Sustain_Scroll(object sender, EventArgs e)
        {
            RebuildEnv();

            pictureBox1.Invalidate();
        }

        private void Release_Scroll(object sender, EventArgs e)
        {

            RebuildEnv();

            pictureBox1.Invalidate();
        }

        private void RebuildEnv()
        {
            Envelope Envelope2 = new Envelope();
            TestFrameLoader.ResetStatic();
            for (int i = 0; i < Math.Min(4000, pictureBox1.Width); i++)
            {
                if (i == 10)
                {
                    Envelope2.TriggerOn(); TestFrameLoader.Trigger(1, 1, (int)EnvMode);
                }
                if (i == 210)
                {
                    Envelope2.TriggerOff(EnvMode); TestFrameLoader.Trigger(0, 1, (int)EnvMode);
                }

                double D = 0;
                triggers2[i] = Envelope2.Trigger;
                values2[i] = TestFrameLoader.GetEnv(1, Attack.Value, Decay.Value, Sustain.Value, Release.Value, Curvature.Value, (int)TimeScale, (int)EnvMode) / 4095.0;
                D = TestFrameLoader.GetEnvCurve(1) / 4095.0;
                linvalues2[i] = D;
            }

        }

        private void Curvature_Scroll(object sender, EventArgs e)
        {
            RebuildEnv();

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

            for (int i = 0; i < 4; i++)
            {
                int B = TestFrameLoader.GetGate(i);
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

            for (int i = 0; i < 13; i++)
            {
                int B = TestFrameLoader.GetLed(i);
                var C = Color.FromArgb(B, B, 0);
                Rectangle R = new Rectangle();
                R.Width = 10;
                R.Height = 10;
                R.X = 2 + i * 12;
                R.Y = 14;
                g.FillRectangle(new SolidBrush(C), R);
                g.DrawRectangle(new Pen(Color.White, 1), R);
            }
            Pen EnvPen = new Pen(Color.FromArgb(200, 200, 200));
            g.DrawLine(EnvPen, 5 + 2 + 12 * 0, 12, 5 + 2 + 12 * 3, 2);
            g.DrawLine(EnvPen, 5 + 2 + 12 * 3, 2, 5 + 2 + 12 * 6, 7);
            g.DrawLine(EnvPen, 5 + 2 + 12 * 6, 7, 5 + 2 + 12 * 9, 7);
            g.DrawLine(EnvPen, 5 + 2 + 12 * 9, 7, 5 + 2 + 12 * 12, 12);
        }

        float ScaleVal(double inp)
        {
            return 20 + (float)((pictureBox1.Height / 2 - 40) * (1.0 - inp));
        }

        List<double> values = new List<double>(4000);
        List<bool> triggers = new List<bool>(4000);
        List<double> linvalues = new List<double>(4000);

        List<double> values2 = new List<double>(4000);
        List<bool> triggers2 = new List<bool>(4000);
        List<double> linvalues2 = new List<double>(4000);
        int pos = 0;

        private void pictureBox1_Resize(object sender, EventArgs e)
        {
            RebuildEnv();
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
                triggers[pos] = Envelope.Trigger;
                values[pos] = TestFrameLoader.GetEnv(0, Attack.Value, Decay.Value, Sustain.Value, Release.Value, Curvature.Value, (int)TimeScale, (int)EnvMode) / 4095.0;
                D = TestFrameLoader.GetEnvCurve(0) / 4095.0;

                linvalues[pos] = D;
            }
        }

        Envelope.EnvelopeMode EnvMode = Envelope.EnvelopeMode.Gate;
        Envelope.EnvelopeTimeScale TimeScale = Envelope.EnvelopeTimeScale.Short;

        private void DispUpdate_Tick(object sender, EventArgs e)
        {
            pictureBox1.Invalidate();
        }
        Envelope Envelope = new Envelope();

        private void button1_MouseDown(object sender, MouseEventArgs e)
        {
            Envelope.TriggerOn();
            TestFrameLoader.Trigger(1, 0, (int)EnvMode);
        }

        private void button1_MouseUp(object sender, MouseEventArgs e)
        {
            Envelope.TriggerOff(EnvMode);
            TestFrameLoader.Trigger(0, 0, (int)EnvMode);

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }
    }

    class Envelope
    {
        public enum EnvelopeMode
        {
            Trigger,
            Gate,
            Loop,
            None

        }
        public enum EnvelopeTimeScale
        {
            Short,
            Long,
            None
        }

        public enum State
        {
            Attack,
            Decay,
            Sustain,
            Release,
            InActive
        }

        State CurrentState = State.InActive;
        double Value = 0;
        double StateTime = 0;

        double GetStateLength(double inp, EnvelopeTimeScale scale)
        {
            if (scale == EnvelopeTimeScale.Long) inp *= 10;
            return (inp * 200) + 1;
        }

        public double Get(out double linear, double attack, double decay, double sustain, double release, double curvature, EnvelopeTimeScale timeScale, EnvelopeMode envMode)
        {
            if (CurrentState != State.InActive) StateTime++;
            switch (CurrentState)
            {
                case State.Attack:
                    {
                        double L = GetStateLength(attack, timeScale);
                        double P = StateTime / L;

                        Value += 1.0 / L;
                        if (Value >= 1)
                        {
                            Value = 1.0;
                            DecayStart = 1.0;
                            CurrentState = State.Decay;
                            StateTime = 0;
                        }
                        linear = Value;
                        return Curved(Value, curvature);
                    }

                case State.Decay:
                    {
                        double L = GetStateLength(decay, timeScale);
                        double P = StateTime / L;

                        Value -= 1.0 / L;
                        if (Value <= sustain)
                        {
                            Value = sustain;

                            if (envMode == EnvelopeMode.Trigger || envMode == EnvelopeMode.Loop)
                            {
                                ReleaseStart = Value;
                                CurrentState = State.Release;
                                StateTime = 0;
                            }
                            else
                            {
                                CurrentState = State.Sustain;
                                StateTime = 0;
                            }
                        }
                        linear = Value;
                        return sustain + Curved((Value - sustain) / (DecayStart - sustain), curvature) * (1 - sustain);
                    }

                case State.Sustain:
                    {
                        StateTime = 0;
                        linear = sustain;
                        return sustain;
                    }

                case State.Release:
                    {
                        double L = GetStateLength(release, timeScale);
                        double P = StateTime / L;

                        Value -= 1.0 / L;

                        if (Value <= 0)
                        {
                            Value = 0;

                            if (envMode == EnvelopeMode.Loop && Trigger)
                            {
                                CurrentState = State.Attack;
                                StateTime = 0;
                            }
                            else
                            {
                                CurrentState = State.InActive;
                                StateTime = 0;
                            }
                        }
                        linear = Value;
                        return Curved(Value / (1 - sustain), curvature) * sustain;
                    }

                case State.InActive: linear = 0; return 0;
            }
            linear = 0;
            return 0;
        }

        private double Curved(double value, double curvature)
        {
            return Math.Pow(value, curvature + 1);
        }



        public bool Trigger = false;
        public double ReleaseStart = 0;
        public double DecayStart = 0;
        internal void TriggerOn()
        {
            if (Trigger == false)
            {
                CurrentState = State.Attack;
                StateTime = 0;
            }
            Trigger = true;
        }

        internal void TriggerOff(EnvelopeMode envMode)
        {
            if (envMode != EnvelopeMode.Trigger && CurrentState != State.InActive && CurrentState != State.Release)
            {
                if (CurrentState == State.Attack)
                {
                    DecayStart = Value;
                }
                CurrentState = State.Release;

                ReleaseStart = Value;
                StateTime = 0;
            }
            Trigger = false;
        }
    }
}
