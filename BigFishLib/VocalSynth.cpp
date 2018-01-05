

#include <math.h>
#include <stdint.h>



#pragma pack (push, 1)

struct Phoneme
{
	float f1f, f1b;
	float f2f, f2b;
	float f3f, f3b;
	float fnf;
	float  a_voicing, a_aspiration, a_frication, a_bypass;
	float  a_1, a_2, a_3, a_4, a_n, a_56;
	float  duration, rank;
};

#pragma pack (pop)

#define NPHONEMES 69
#define PTABSIZE  1311

static const unsigned char multipliers[PTABSIZE / NPHONEMES] = { 10,10,20,10,20,10,10,1,1,1,1,1,1,1,1,1,1,1,1 };

static const unsigned char rawphonemes[PTABSIZE] =
{
	//   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40  41  42  43  44  45  46  47  48  49  50  51  52  53  54  55  56  57  58  59  60  61  62  63  64  65  66  67  68 
	// f1f (Hz*1/10)
	49, 30,241,  0, 15,226,226,  0,  0,  0, 21,235,  9,  0,247,  0, 45,217, 24,  9,238,235,  0,  0, 30,247,247, 18, 30,196,  9,247,  0,  0, 27,  0,  2,  0,  0, 13,244,  0,244,  0, 12, 30,214,238,  0,  0, 30,235, 21,  0,247,  0,235, 21,235,  0, 51,211,  3,247,  0,  6,  3,  0,  0,
	// f1b (Hz*1/10)
	234,  7,249,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,254,  0, 12,246,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 14,242,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	// f2f (Hz*1/20)
	68, 15,247, 21,199,253,253,  0,  0, 51, 12,244,247,  0,  9,  0, 12, 15,214,253,  0,  3,  0,  0,  0, 30,  6,220,226, 45, 12,229,  0,  0,  0,229,  3, 39,208,  3, 30,223,  9,  0,247, 24,241,244,  0,  0, 36,253,244,  0, 27, 24,235,  0,  0,  0,235,232, 27,223, 36, 51,217, 15,241,
	// f2b (Hz*1/10)
	179,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8, 13,241,250,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	// f3f (Hz*1/20)
	116,  0,  0,  0,  0,  0,  0,  0,  0,  9,250,  6,250,  0,  6,  0,247, 35,221,  0,  3,  3,  0,  0,250,  3, 18,235,  0,  9,250,  3,  0,  0,250,  0,241, 21,  9,241,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,208,  0, 51,253,  6,  0,  0,  0,247,241, 18,229, 30, 18,235,  0,  0,
	// f3b (Hz*1/10)
	143,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,253, 14,240,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,248, 15,249,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	// fnf (Hz*1/10)
	12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,  9,247,247,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	// a_voicing (dB)
	35,  0,  0,  0,  0,  0,  0,  0,  0,194,  0, 62,230, 26,  0,  0,  0,  0,194, 62,194, 62,  0,  0,194, 62,  0,  0,  0,  0,  0,194,  0,  0, 62,  0,  0,  0,246, 10,  0,  0,  0,  0,  0,  0,  0,194,  0,  0,  0, 62,  0,244,206,  0,  0,  0,  0,  0, 62,  0,  0,  0,194, 62,234, 22,  0,
	// a_aspiration (dB)
	194,  0,  0,  0,  0,  0,  0,  0,  0, 60,  0,196,  0,  0,  0,  0,  0,  0,  0,  0, 32,224,  0,  0, 60,196,  0,  0,  0,  0,  0, 60,  0,  0,196,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 60,  0,  0,196,  0,  0,  0, 32, 28,  0,  0,  0,  0,196,  0,  0,  0, 60,196, 20,236,  0,
	// a_frication (dB)
	0,  0,  0,  0,  0,  0,  0,  0,  0, 60,  0,196, 60,196,  0,  0,  0,  0,  0,  0, 54,202,  0,  0, 60,196,  0,  0,  0,  0,  0, 60,  0,  0,196,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 60,  0,  0,196,  0,  0,  0, 60,  0,  0,  0,  0,  0,196,  0,  0,  0, 60,196, 60,196,  0,
	// a_bypass (dB)
	237,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 70,186,  0,  0,  0,  0,  0,  0, 70,186,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	// a_f1 (dB)
	83,  0,  0,  0,  0,  0,230,  0,202,  0,  0, 61,254,  0,  9,  0, 12,  0,176, 80,176, 65,  0,  0,  0, 15,  0,  0,  0,237,254,197,  0,  0, 66,  0,246,  9,241, 30,  0,  0,  0,  0,  0,  0,  0,176, 54,  0,202, 59, 13,  0,184,  0,  0,  0,  0,  0, 80,  0,235, 14,183, 80,235,  0,  0,
	// a_f2 (dB)
	21,253,254,253,  7,252,181, 79,177,  0, 61,195, 61,  0,  7,246, 14,247,193, 75,211,226, 75,246,  1,  0,255, 15,255,177, 56,200, 80,246,242,  0,  4,  5,251, 17,  3,251,253,  0,  3,  2,251,184, 79,245,188, 70,251,  0,249,  3,195, 56,200,  0, 73,251,  2,244, 12,249,247,  2,254,
	// a_f3 (dB)
	9,  5,253,  3,247,249,204, 73,183,  0, 72,184, 56,  0,  9,245, 14,254,190, 63,223,226, 70,245,253,  9,  1,253,252,197, 66,190, 80,246,242,  0,  7,  2,  0,243, 11,245,  6,  0,250, 13,249,198, 73,246,193, 66,255,  0,249, 14,184, 58, 10,246,  3,242, 19,241, 19,254,242, 12,244,
	// a_f4 (dB)
	2,  5,254,  2,247,251,209, 68,188,  0, 61,195, 58,  0, 17,246,252,  0,195, 56,200,  0, 54,246,  8,  7,  2,251,252,204, 56,200, 59,246,  2,  0,205, 50,236, 15, 11,247,  5,  0,251, 11,250,204, 68,246,198, 63,193,  0, 70,247,195, 52, 28,246,240,242, 23,193, 49, 12,  5,246, 10,
	// a_fn (dB)
	190,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 72,  0,  0,184,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 56,200,  0, 66,246, 10,246,  0,246,210,  0, 56,200, 46,210, 66,236,  0,
	// a_f56 (dB)
	225,  0,  0,  0,  0,  0,  0,  0,  0, 46,246,  0, 10,236,246,240,  0,  0,  0,  0, 46,210,  0,  0, 26,230,  0,  0,  0, 26,246, 10,  0,246,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 46,236, 10,246,230,  0,  0,  0,  0,  0,  0,  0,  0, 26,  0,
	// duration (frames)
	235,  1,  1,  0,  9,251,  2,245,255,  4,  4,  0,252,  0,253,  0,  3,  3,254, 11,252,  0,245,  1,  3,  1,  0,  0,  0,254,255,  5,249,  3,  4,  0,  0,  0,  0,254,  0,  0,254,  2,  0,  0,  0,  2,249,  1,  4,250, 11,255,  2,  0,250,  9,242,  1,  4,  3,251,  4,  4,251,253,  0,  0,
	// rank
	254,  0,  0,  0,  0,  0, 24,  3,253,253,251,  8,250,  0,  9,253,232,  0, 29,227, 16,  8,  3,253,239,249,  0,  0,  0, 24,250,  3,  6,250,244,  0,  0,  0,  0,247,  0,  0,  0,  0,  0,  0,  0, 21,  6,250,  6,  1,236,  0,  8,  0,  5,251, 11,250,235,  0, 18,246,  8,248, 10,  0,  0,
};
/*
static Phoneme orgphonemes[] =
{
{  490,   60, 1480,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 64, 47, 40,-16,-16,  4,  2}, //   0: a
{  790,  130, 1780,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 61, 52, 45,-16,-16,  5,  2}, //   1: aa
{  640,   60, 1600,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 59, 49, 43,-16,-16,  6,  2}, //   2: ai
{  640,   60, 2020,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 56, 52, 45,-16,-16,  6,  2}, //   3: air
{  790,   60,  880,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 63, 43, 36,-16,-16, 15,  2}, //   4: ar
{  490,   60,  820,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 59, 36, 31,-16,-16, 10,  2}, //   5: aw
{  190,   60,  760,   90, 2500,  150,  270, 62,  0,  0,-16, 38,-16,-16,-16,-16,-16, 12, 26}, //   6: b
{  190,   60,  760,   90, 2500,  150,  270, 62,  0,  0,-16, 38, 63, 57, 52,-16,-16,  1, 29}, //   7: by
{  190,   60,  760,   90, 2500,  150,  270, 62,  0,  0,-16,-16,-16,-16,-16,-16,-16,  0, 26}, //   8: bz
{  190,   60, 1780,   90, 2680,  150,  270,  0, 60, 60,-16,-16,-16,-16,-16,-16, 30,  4, 23}, //   9: ch
{  400,   60, 2020,   90, 2560,  150,  270,  0, 60, 60,-16,-16, 45, 56, 45,-16, 20,  8, 18}, //  10: ci
{  190,   60, 1780,   90, 2680,  150,  270, 62,  0,  0,-16, 45,-16,-16,-16,-16, 20,  8, 26}, //  11: d
{  280,   60, 1600,   90, 2560,  150,  270, 36,  0, 60, 54, 43, 45, 40, 42,-16, 30,  4, 20}, //  12: dh
{  280,   60, 1600,   90, 2560,  150,  270, 62,  0,  0,-16, 43, 45, 40, 42,-16, 10,  4, 20}, //  13: di
{  190,   60, 1780,   90, 2680,  150,  270, 62,  0,  0,-16, 52, 52, 49, 59,-16,  0,  1, 29}, //  14: dy
{  190,   60, 1780,   90, 2680,  150,  270, 62,  0,  0,-16, 52, 42, 38, 49,-16,-16,  1, 26}, //  15: dz
{  640,   60, 2020,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 56, 52, 45,-16,-16,  4,  2}, //  16: e
{  250,   60, 2320,   90, 3200,  150,  270, 62,  0,  0,-16, 64, 47, 50, 45,-16,-16,  7,  2}, //  17: ee
{  490,   60, 1480,   90, 2500,  150,  270,  0,  0,  0,-16,-16,-16,-16,-16,-16,-16,  5, 31}, //  18: end
{  580,   60, 1420,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 59, 47, 40,-16,-16, 16,  2}, //  19: er
{  400,   60, 1420,   90, 2560,  150,  270,  0, 32, 54, 54,-16, 14, 14,-16,-16, 30, 12, 18}, //  20: f
{  190,   60, 1480,   90, 2620,  150,  270, 62,  0,  0,-16, 49,-16,-16,-16,-16,-16, 12, 26}, //  21: g
{  190,   60, 1480,   90, 2620,  150,  270, 62,  0,  0,-16, 49, 59, 54, 38,-16,-16,  1, 29}, //  22: gy
{  190,   60, 1480,   90, 2620,  150,  270, 62,  0,  0,-16, 49, 49, 43, 28,-16,-16,  2, 26}, //  23: gz
{  490,   60, 1480,   90, 2500,  150,  270,  0, 60, 60,-16, 49, 50, 40, 36,-16, 10,  5,  9}, //  24: h
{  400,   60, 2080,   90, 2560,  150,  270, 62,  0,  0,-16, 64, 50, 49, 43,-16,-16,  6,  2}, //  25: i
{  310,   60, 2200,   90, 2920,  150,  270, 62,  0,  0,-16, 64, 49, 50, 45,-16,-16,  6,  2}, //  26: ia
{  490,   60, 1480,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 64, 47, 40,-16,-16,  6,  2}, //  27: ib
{  790,   60,  880,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 63, 43, 36,-16,-16,  6,  2}, //  28: ie
{  190,   60, 1780,   90, 2680,  150,  270, 62,  0,  0,-16, 45,-16,-16,-16,-16, 10,  4, 26}, //  29: j
{  280,   60, 2020,   90, 2560,  150,  270, 62,  0,  0,-16, 43, 40, 50, 40,-16,  0,  3, 20}, //  30: jy
{  190,   60, 1480,   90, 2620,  150,  270,  0, 60, 60,-16,-16,-16,-16,-16,-16, 10,  8, 23}, //  31: k
{  190,   60, 1480,   90, 2620,  150,  270,  0, 60, 60,-16,-16, 64, 64, 43,-16, 10,  1, 29}, //  32: ky
{  190,   60, 1480,   90, 2620,  150,  270,  0, 60, 60,-16,-16, 54, 54, 33,-16,  0,  4, 23}, //  33: kz
{  460,   60, 1480,   90, 2500,  150,  270, 62,  0,  0,-16, 50, 40, 40, 35,-16,-16,  8, 11}, //  34: l
{  460,   60,  940,   90, 2500,  150,  270, 62,  0,  0,-16, 50, 40, 40, 35,-16,-16,  8, 11}, //  35: ll
{  480,   40, 1000,  170, 2200,  120,  360, 62,  0,  0,-16, 40, 44, 47,-16, 56,-16,  8, 11}, //  36: m
{  480,   40, 1780,  300, 2620,  260,  450, 62,  0,  0,-16, 49, 49, 49, 34, 56,-16,  8, 11}, //  37: n
{  480,  160,  820,  150, 2800,  100,  360, 52,  0,  0,-16, 34, 44, 49, 14, 56,-16,  8, 11}, //  38: ng
{  610,   60,  880,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 61, 36, 29,-16,-16,  6,  2}, //  39: o
{  490,   60, 1480,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 64, 47, 40,-16,-16,  6,  2}, //  40: oa
{  490,   60,  820,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 59, 36, 31,-16,-16,  6,  2}, //  41: oi
{  370,   60, 1000,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 56, 42, 36,-16,-16,  4,  2}, //  42: oo
{  370,   60, 1000,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 56, 42, 36,-16,-16,  6,  2}, //  43: oor
{  490,   60,  820,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 59, 36, 31,-16,-16,  6,  2}, //  44: or
{  790,   60, 1300,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 61, 49, 42,-16,-16,  6,  2}, //  45: ou
{  370,   60, 1000,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 56, 42, 36,-16,-16,  6,  2}, //  46: ov
{  190,   60,  760,   90, 2500,  150,  270,  0, 60, 60,-16,-16,-16,-16,-16,-16,-16,  8, 23}, //  47: p
{  190,   60,  760,   90, 2500,  150,  270,  0, 60, 60,-16, 38, 63, 57, 52,-16,-16,  1, 29}, //  48: py
{  190,   60,  760,   90, 2500,  150,  270,  0, 60, 60,-16, 38, 52, 47, 42,-16,-16,  2, 23}, //  49: pz
{  490,   60, 1480,   90, 2500,  150,  270,  0,  0,  0,-16,-16,-16,-16,-16,-16,-16,  6, 29}, //  50: q
{  280,   60, 1420,   90, 2560,  150,  270, 62,  0,  0,-16, 43, 54, 50, 47, 40,-16,  0, 30}, //  51: qq
{  490,   60, 1180,   90, 1600,  150,  270, 62,  0,  0,-16, 56, 49, 49,-16,-16,-16, 11, 10}, //  52: r
{  490,   60, 1180,   90, 1600,   70,  270, 50,  0,  0,-16, 56, 49, 49,-16,-16,-16, 10, 10}, //  53: rx
{  400,  200, 1720,   90, 2620,  220,  270,  0, 32, 60,-16,-16, 42, 42, 54, 50, 30, 12, 18}, //  54: s
{  400,   60, 2200,   90, 2560,  150,  270,  0, 60, 60,-16,-16, 45, 56, 45, 40, 10, 12, 18}, //  55: sh
{  190,   60, 1780,   90, 2680,  150,  270,  0, 60, 60,-16,-16,-16,-16,-16, 50, 20,  6, 23}, //  56: t
{  400,   60, 1780,   90, 2680,  150,  270,  0, 60, 60,-16,-16, 40, 42, 36, 40, 10, 15, 18}, //  57: th
{  190,   60, 1780,   90, 2680,  150,  270,  0, 60, 60,-16,-16,-16, 52, 64, 40,-16,  1, 29}, //  58: ty
{  190,   60, 1780,   90, 2680,  150,  270,  0, 60, 60,-16,-16,-16, 42, 54, 30,-16,  2, 23}, //  59: tz
{  700,   60, 1360,   90, 2500,  150,  270, 62,  0,  0,-16, 64, 57, 45, 38,-16,-16,  6,  2}, //  60: u
{  250,   60,  880,   90, 2200,  150,  270, 62,  0,  0,-16, 64, 52, 31, 24,-16,-16,  9,  2}, //  61: uu
{  280,   60, 1420,   90, 2560,  150,  270, 62,  0,  0,-16, 43, 54, 50, 47, 40,-16,  4, 20}, //  62: v
{  190,   60,  760,   90, 2020,  150,  270, 62,  0,  0,-16, 57, 42, 35,-16,-16,-16,  8, 10}, //  63: w
{  190,   60, 1480,   90, 2620,  150,  270,  0, 60, 60,-16,-16, 54, 54, 33, 30,-16, 12, 18}, //  64: x
{  250,   60, 2500,   90, 2980,  150,  270, 62,  0,  0,-16, 64, 47, 52, 45,-16,-16,  7, 10}, //  65: y
{  280,   60, 1720,   90, 2560,  150,  270, 40, 20, 60,-16, 43, 38, 38, 50, 50,-16,  4, 20}, //  66: z
{  280,   60, 2020,   90, 2560,  150,  270, 62,  0,  0,-16, 43, 40, 50, 40, 30, 10,  4, 20}, //  67: zh
{  280,   60, 1720,   90, 2560,  150,  270, 62,  0,  0,-16, 43, 38, 38, 50, 30, 10,  4, 20}, //  68: zz
};
*/


static int sFtol(const float f)
{
	return (int)f;
}

static float sFPow(double a, double b)
{
	return powf(a, b);
}

static float sFExp(double f)
{
	return expf(f);
}

#ifndef sCopyMem
#define sCopyMem memcpy
#endif



int mystrnicmp1(const char *a, const char *b)
{
	int l = 0;
	while (*a && *b)
		if ((*a++ | 0x20) != (*b++ | 0x20))
			return 0;
		else
			l++;
	return *a ? 0 : l;
}


#define PI (4.0f*(float)atan(1.0f))


static Phoneme phonemes[NPHONEMES];

static struct
{
	uint32_t samplerate;
	float fcminuspi_sr;
	float fc2pi_sr;
} g;


static const char *nix = "";

static const struct syldef
{
	char syl[4];
	char  ptab[4];
} syls[] = {
	{ "sil",{ 50,-1,-1,-1 } },
	{ "ng",{ 38,-1,-1,-1 } },
	{ "th",{ 57,-1,-1,-1 } },
	{ "sh",{ 55,-1,-1,-1 } },
	{ "dh",{ 12,51,13,-1 } },
	{ "zh",{ 67,51,67,-1 } },
	{ "ch",{ 9,10,-1,-1 } },
	{ "ih",{ 25,-1,-1,-1 } },
	{ "eh",{ 16,-1,-1,-1 } },
	{ "ae",{ 1,-1,-1,-1 } },
	{ "ah",{ 60,-1,-1,-1 } },
	{ "oh",{ 39,-1,-1,-1 } },
	{ "uh",{ 42,-1,-1,-1 } },
	{ "ax",{ 0,-1,-1,-1 } },
	{ "iy",{ 17,-1,-1,-1 } },
	{ "er",{ 19,-1,-1,-1 } },
	{ "aa",{ 4,-1,-1,-1 } },
	{ "ao",{ 5,-1,-1,-1 } },
	{ "uw",{ 61,-1,-1,-1 } },
	{ "ey",{ 2,25,-1,-1 } },
	{ "ay",{ 28,25,-1,-1 } },
	{ "oy",{ 41,25,-1,-1 } },
	{ "aw",{ 45,46,-1,-1 } },
	{ "ow",{ 40,46,-1,-1 } },
	{ "ia",{ 26,27,-1,-1 } },
	{ "ea",{ 3,27,-1,-1 } },
	{ "ua",{ 43,27,-1,-1 } },
	{ "ll",{ 35,-1,-1,-1 } },
	{ "wh",{ 63,-1,-1,-1 } },
	{ "ix",{ 0,-1,-1,-1 } },
	{ "el",{ 34,-1,-1,-1 } },
	{ "rx",{ 53,-1,-1,-1 } },
	{ "h",{ 24,-1,-1,-1 } },
	{ "p",{ 47,48,49,-1 } },
	{ "t",{ 56,58,59,-1 } },
	{ "k",{ 31,32,33,-1 } },
	{ "b",{ 6, 7, 8,-1 } },
	{ "d",{ 11,14,15,-1 } },
	{ "g",{ 21,22,23,-1 } },
	{ "m",{ 36,-1,-1,-1 } },
	{ "n",{ 37,-1,-1,-1 } },
	{ "f",{ 20,-1,-1,-1 } },
	{ "s",{ 54,-1,-1,-1 } },
	{ "v",{ 62,51,62,-1 } },
	{ "z",{ 66,51,68,-1 } },
	{ "l",{ 34,-1,-1,-1 } },
	{ "r",{ 52,-1,-1,-1 } },
	{ "w",{ 63,-1,-1,-1 } },
	{ "q",{ 51,-1,-1,-1 } },
	{ "y",{ 65,-1,-1,-1 } },
	{ "j",{ 29,30,51,30 } },
	{ " ",{ 18,-1,-1,-1 } },
};
#define NSYLS (sizeof(syls)/sizeof(syldef))



// filter type 1: 2-pole resonator
struct ResDef
{
	float a, b, c;  // coefficients

	void set(float f, float bw, float gain)
	{
		float r = (float)sFExp(g.fcminuspi_sr*bw);
		c = -(r*r);
		b = r*(float)cos(g.fc2pi_sr*f)*2.0f;
		a = gain*(1.0f - b - c);
	}
};

struct Resonator
{
	ResDef *def;
	float p1, p2; // delay buffers

	inline void setdef(ResDef &a_def) { def = &a_def; }

	float tick(float in)
	{
		float x = def->a*in + def->b*p1 + def->c*p2;
		p2 = p1;
		p1 = x;
		return x;
	}
};


static ResDef d_peq1;

static float flerp(const float a, const float b, const float x) { return a + x*(b - a); }
static float db2lin(float db1, float db2, float x) { return (float)sFPow(2.0, (flerp(db1, db2, x) - 70) / 6.0); }
static const float f4 = 3200;
static const float f5 = 4000;
static const float f6 = 6000;
static const float bn = 100;
static const float b4 = 200;
static const float b5 = 500;
static const float b6 = 800;

struct syVRonan
{
	ResDef rdef[7]; // nas,f1,f2,f3,f4,f5,f6;
	float a_voicing;
	float a_aspiration;
	float a_frication;
	float a_bypass;
};


struct syWRonan : syVRonan
{
	syVRonan newframe;

	Resonator res[7];  // 0:nas, 1..6: 1..6

	float lastin2;

	// settings
	const char *texts[64];
	float  pitch;
	int  framerate;

	// noise
	uint32_t nseed;
	float nout;

	// phonem seq
	int framecount;  // frame rate divider
	int spos;        // pos within syl definition (0..3)
	int scounter;    // syl duration divider
	int cursyl;      // current syl
	int durfactor;   // duration modifier
	float invdur;      // 1.0 / current duration
	const char *baseptr; // pointer to start of text
	const char *ptr;  // pointer to text
	int curp1, curp2;  // current/last phonemes

						// sync
	int wait4on;
	int wait4off;

	// post EQ
	float hpb1, hpb2;
	Resonator peq1;

	void SetFrame(const Phoneme &p1s, const Phoneme &p2s, const float x, syVRonan &dest)
	{
		static Phoneme p1, p2;

		static const float * const p1f[] = { &p1.fnf,&p1.f1f,&p1.f2f,&p1.f3f,&f4    ,&f5     ,&f6 };
		static const float * const p1b[] = { &bn    ,&p1.f1b,&p1.f2b,&p1.f3b,&b4    ,&b5     ,&b6 };
		static const float * const p1a[] = { &p1.a_n,&p1.a_1,&p1.a_2,&p1.a_3,&p1.a_4,&p1.a_56,&p1.a_56 };

		static const float * const p2f[] = { &p2.fnf,&p2.f1f,&p2.f2f,&p2.f3f,&f4    ,&f5     ,&f6 };
		static const float * const p2b[] = { &bn    ,&p2.f1b,&p2.f2b,&p2.f3b,&b4    ,&b5     ,&b6 };
		static const float * const p2a[] = { &p2.a_n,&p2.a_1,&p2.a_2,&p2.a_3,&p2.a_4,&p2.a_56,&p2.a_56 };

		p1 = p1s;
		p2 = p2s;

		for (int i = 0; i < 7; i++)
			dest.rdef[i].set(flerp(*p1f[i], *p2f[i], x)*pitch, flerp(*p1b[i], *p2b[i], x), db2lin(*p1a[i], *p2a[i], x));

		dest.a_voicing = db2lin(p1.a_voicing, p2.a_voicing, x);
		dest.a_aspiration = db2lin(p1.a_aspiration, p2.a_aspiration, x);
		dest.a_frication = db2lin(p1.a_frication, p2.a_frication, x);
		dest.a_bypass = db2lin(p1.a_bypass, p2.a_bypass, x);
	}


#define NOISEGAIN 0.25f
	float noise()
	{
		union { uint32_t i; float f; } val;

		// random...
		nseed = (nseed * 196314165) + 907633515;

		// convert to float between 2.0 and 4.0
		val.i = (nseed >> 9) | 0x40000000;

		// slight low pass filter...
		nout = (val.f - 3.0f) + 0.75f*nout;
		return nout*NOISEGAIN;
	}

	void reset()
	{
		for (int i = 0; i < 7; i++) res[i].setdef(rdef[i]);
		peq1.setdef(d_peq1);
		SetFrame(phonemes[18], phonemes[18], 0, *this); // off
		SetFrame(phonemes[18], phonemes[18], 0, newframe); // off
		curp1 = curp2 = 18;
		spos = 4;
	}


};


void __stdcall ronanCBSetSR(syWRonan *ptr, uint32_t sr)
{
	g.samplerate = sr;
	g.fc2pi_sr = 2.0f*PI / (float)sr;
	g.fcminuspi_sr = -g.fc2pi_sr*0.5f;
}


extern "C" void __stdcall ronanCBInit(syWRonan *wsptr)
{
	// convert phoneme table to a usable format
	register const char *ptr = (const char*)rawphonemes;
	register int32_t val = 0;
	for (int f = 0; f < (PTABSIZE / NPHONEMES); f++)
	{
		float *dest = ((float*)phonemes) + f;
		for (int p = 0; p < NPHONEMES; p++)
		{
			*dest = multipliers[f] * (float)(val += *ptr++);
			dest += PTABSIZE / NPHONEMES;
		}
	}

	wsptr->reset();

	wsptr->framerate = 3;
	wsptr->pitch = 1.0f;

	if (!wsptr->texts[0])
		wsptr->baseptr = wsptr->ptr = nix;
	else
		wsptr->baseptr = wsptr->ptr = wsptr->texts[0];

	/*wsptr->lastin=*/wsptr->lastin2 =/*wsptr->nval=*/0;

	d_peq1.set(12000, 4000, 2.0f);

}


extern "C" void __stdcall ronanCBTick(syWRonan *wsptr)
{
	if (wsptr->wait4off || wsptr->wait4on) return;

	if (!wsptr->ptr) return;

	if (wsptr->framecount <= 0)
	{
		wsptr->framecount = wsptr->framerate;
		// let current phoneme expire
		if (wsptr->scounter <= 0)
		{
			// set to next phoneme
			wsptr->spos++;
			if (wsptr->spos >= 4 || syls[wsptr->cursyl].ptab[wsptr->spos] == -1)
			{
				// go to next syllable

				if ((wsptr->ptr == 0) || (wsptr->ptr[0] == 0)) // empty text: silence!
				{
					wsptr->durfactor = 1;
					wsptr->framecount = 1;
					wsptr->cursyl = NSYLS - 1;
					wsptr->spos = 0;
					wsptr->ptr = wsptr->baseptr;
				}
				else if (*wsptr->ptr == '!') // wait for noteon
				{
					wsptr->framecount = 0;
					wsptr->scounter = 0;
					wsptr->wait4on = 1;
					wsptr->ptr++;
					return;
				}
				else if (*wsptr->ptr == '_') // noteoff
				{
					wsptr->framecount = 0;
					wsptr->scounter = 0;
					wsptr->wait4off = 1;
					wsptr->ptr++;
					return;
				}

				if (*wsptr->ptr && *wsptr->ptr != '!' && *wsptr->ptr != '_')
				{
					wsptr->durfactor = 0;
					while (*wsptr->ptr >= '0' && *wsptr->ptr <= '9') wsptr->durfactor = 10 * wsptr->durfactor + (*wsptr->ptr++ - '0');
					if (!wsptr->durfactor) wsptr->durfactor = 1;

					//					printf2("'%s' -> ",wsptr->ptr);

					int fs, len = 1, len2;
					for (fs = 0; fs < NSYLS - 1; fs++)
					{
						const syldef &s = syls[fs];
						if (len2 = mystrnicmp1(s.syl, wsptr->ptr))
						{
							len = len2;
							//							printf2("got %s\n",s.syl);
							break;
						}
					}
					wsptr->cursyl = fs;
					wsptr->spos = 0;
					wsptr->ptr += len;
				}
			}

			wsptr->curp1 = wsptr->curp2;
			wsptr->curp2 = syls[wsptr->cursyl].ptab[wsptr->spos];
			wsptr->scounter = sFtol(phonemes[wsptr->curp2].duration*wsptr->durfactor);
			if (!wsptr->scounter) wsptr->scounter = 1;
			wsptr->invdur = 1.0f / ((float)wsptr->scounter*wsptr->framerate);
		}
		wsptr->scounter--;
	}

	wsptr->framecount--;
	float x = (float)(wsptr->scounter*wsptr->framerate + wsptr->framecount)*wsptr->invdur;
	const Phoneme &p1 = phonemes[wsptr->curp1];
	const Phoneme &p2 = phonemes[wsptr->curp2];
	x = (float)sFPow(x, (float)p1.rank / (float)p2.rank);
	wsptr->SetFrame(p2, (fabs(p2.rank - p1.rank) > 8.0f) ? p2 : p1, x, wsptr->newframe);

}

extern "C" void __stdcall ronanCBNoteOn(syWRonan *wsptr)
{
	wsptr->wait4on = 0;
}

extern "C" void __stdcall ronanCBNoteOff(syWRonan *wsptr)
{
	wsptr->wait4off = 0;
}


extern "C" void __stdcall ronanCBSetCtl(syWRonan *wsptr, uint32_t ctl, uint32_t val)
{
	// controller 4, 0-63			: set text #
	// controller 4, 64-127		: set frame rate
	// controller 5					: set pitch
	switch (ctl)
	{
	case 4:
		if (val < 63)
		{
			wsptr->reset();

			if (wsptr->texts[val])
				wsptr->ptr = wsptr->baseptr = wsptr->texts[val];
			else
				wsptr->ptr = wsptr->baseptr = nix;
		}
		else
			wsptr->framerate = val - 63;
		break;
	case 5:
		wsptr->pitch = (float)sFPow(2.0f, (val - 64.0f) / 128.0f);
		break;

	}
}


extern "C" void __stdcall ronanCBProcess(syWRonan *wsptr, float *buf, uint32_t len)
{
	static syVRonan deltaframe;

	// prepare interpolation
	{
		float *src1 = (float*)wsptr;
		float *src2 = (float*)&wsptr->newframe;
		float *dest = (float*)&deltaframe;
		float mul = 1.0f / (float)len;
		for (uint32_t i = 0; i < (sizeof(syVRonan) / sizeof(float)); i++)
			dest[i] = (src2[i] - src1[i])*mul;
	}

	for (uint32_t i = 0; i < len; i++)
	{
		// interpolate all values
		{
			float *src = (float*)&deltaframe;
			float *dest = (float*)wsptr;
			for (uint32_t i = 0; i < (sizeof(syVRonan) / sizeof(float)); i++)
				dest[i] += src[i];
		}

		float in = buf[2 * i];

		// add aspiration noise
		in = in*wsptr->a_voicing + wsptr->noise()*wsptr->a_aspiration;

		// process complete input signal with f1/nasal filters
		float out = wsptr->res[0].tick(in) + wsptr->res[1].tick(in);


		// differentiate input signal, add frication noise
		float lin = in;
		in = (wsptr->noise()*wsptr->a_frication) + in - wsptr->lastin2;
		wsptr->lastin2 = lin;

		// process diff/fric input signal with f2..f6 and bypass (phase inverted)
		for (int r = 2; r < 7; r++)
			out = wsptr->res[r].tick(in) - out;

		out = in*wsptr->a_bypass - out;

		// high pass filter
		wsptr->hpb1 += 0.012f*(out = out - wsptr->hpb1);
		wsptr->hpb2 += 0.012f*(out = out - wsptr->hpb2);

		// EQ
		out = wsptr->peq1.tick(out) - out;
		buf[2 * i] = buf[2 * i + 1] = out;
	}
}

extern "C" extern void* __stdcall synthGetSpeechMem(void *a_pthis);

extern "C" void __stdcall synthSetLyrics(void *a_pthis, const char **a_ptr)
{
	syWRonan *wsptr = (syWRonan*)synthGetSpeechMem(a_pthis);
	for (int i = 0; i < 64; i++) wsptr->texts[i] = a_ptr[i];
	wsptr->baseptr = wsptr->ptr = wsptr->texts[0];
}

