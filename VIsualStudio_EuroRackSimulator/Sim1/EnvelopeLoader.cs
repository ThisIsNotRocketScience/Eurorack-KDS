using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace Sim1
{
    class TestFrameLoader
    {


        #region TUESDAY

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Tuesday_GetPatternLength();

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Tuesday_GetTickVel(int tick);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Tuesday_GetTickNote(int tick);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Tuesday_GetTickGate(int tick);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Tuesday_GetTickAccent(int tick);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Tuesday_UpdatePattern(int algo, int scale, int ticks, int beats, int tempo, int x, int y, int i);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Tuesday_ClockTick(int ClockConnected, int state);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Tuesday_DoTimer();

        #endregion


        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Init();

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool Tuesday_GetAccent();
        
        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Tuesday_GetVel();

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Tuesday_GetCV();

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool Tuesday_GetGate();
        
        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void ResetStatic();

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetEnv(int staticenv, int attack, int decay, int sustain, int release, int curvature, int speed, int mode);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetEnvCurve(int staticenv);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Trigger(int value, int staticenv, int mode);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetGate(int gate);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetLed(int led);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetLFOLed(int led);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetLFO(int staticlfo, int speed, int shape, int mod, int phaseing);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetLFOGate(int gate);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetLFOPhased(int staticlfo);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void LFOTrigger(int value, int staticlfo);


    }
}
