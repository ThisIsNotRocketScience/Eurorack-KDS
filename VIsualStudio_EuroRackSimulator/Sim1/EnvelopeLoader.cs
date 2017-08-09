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

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Init();

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void ResetStatic();

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Trigger(int value, int staticenv, int mode);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetGate(int gate);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetLed(int led);

        #region TUESDAY
        public enum SCALENAMES
        {
            SCALE_MAJOR,
            SCALE_MINOR,
            SCALE_DORIAN,
            SCALE_BLUES,
            SCALE_PENTA,
            SCALE_12TONE,
            SCALE_MAJORTRIAD,
            SCALE_MINORTRIAD,
            SCALE_9,
            SCALE_10,
            SCALE_11,
            SCALE_12,
            SCALE_13,
            SCALE_14,
            SCALE_15,
            SCALE_16,
            __SCALE_COUNT
        }

        public enum ALGONAMES {
            ALGO_TESTS,

            // german style minimal melodies
            ALGO_TRITRANCE,

            // fishyfishy! with slides! 
            ALGO_STOMPER,

            // Random walking with mr. Markov.
            ALGO_MARKOV,

            //	ALGO_PACHEDECO,
            ALGO_WOBBLE,
            ALGO_CHIPARP1,
            ALGO_CHIPARP2,

            ALGO_SNH,
            // classic saiko things, reimagined
            ALGO_SAIKO_LEAD,
            ALGO_SAIKO_CLASSIC,
            ALGO_SCALEWALKER,
            ALGO_TOOEASY,
            ALGO_RANDOM,

            __ALGO_COUNT  

        }

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Compare();

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
        public static extern int Tuesday_GetTickSlide(int tick);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Tuesday_GetTickLength(int tick);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Tuesday_UpdatePattern(int algo, int scale, int ticks, int beats, int tempo, int x, int y, int i);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Tuesday_ClockTick(int ClockConnected, int state);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern void Tuesday_DoTimer();
        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool Tuesday_GetAccent();

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Tuesday_GetVel();

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int Tuesday_GetCV();

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern bool Tuesday_GetGate();

        #endregion

        #region EDGECUTTER
        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetEnv(int staticenv, int attack, int decay, int sustain, int release, int curvature, int speed, int mode);

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern int GetEnvCurve(int staticenv);
        #endregion

        #region WOBBLER

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

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern float RunPendulum();

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern float RunPendulum2();

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.StdCall)]
        public static extern Int32 RunPendulumInt();

        [DllImport(@"TestFrameDLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern Int32 RunPendulum2Int();

        #endregion
    }
}
