#include "GoldfishConfig.h"
#ifdef GOLDFISHSYNTH

#pragma GCC optimize ("O3")

#include "Goldfish_v2.h"
#include <math.h>



CMachineParameter const *ppcParameters[] =
{
	// global
	&paraBypass,
	&paraPush,
	&paraSound
};

<<<<<<< HEAD
#include "saw.h"
#include "pulse.h"
=======
#include "Saw.h"
#define USEPULSETOO
#ifdef USEPULSETOO
#include "Pulse.h"
#endif
//#include "saw.h"
//#include "pulse.h"
#ifndef BUZZ

>>>>>>> origin/master


const unsigned short DecayTime[] = {
	0x0002, 0x0003, 0x0003, 0x0004, 0x0005, 0x0005, 0x0006, 0x0007, 0x0007, 0x0008, 0x0009, 0x000A, 0x000B, 0x000C, 0x000C, 0x000D,
	0x000E, 0x000F, 0x0010, 0x0012, 0x0013, 0x0014, 0x0015, 0x0016, 0x0018, 0x0019, 0x001B, 0x001C, 0x001E, 0x001F, 0x0021, 0x0023,
	0x0024, 0x0026, 0x0028, 0x002A, 0x002C, 0x002E, 0x0031, 0x0033, 0x0035, 0x0038, 0x003A, 0x003D, 0x003F, 0x0042, 0x0045, 0x0048,
	0x004B, 0x004E, 0x0051, 0x0055, 0x0058, 0x005C, 0x005F, 0x0063, 0x0067, 0x006B, 0x006F, 0x0073, 0x0078, 0x007C, 0x0081, 0x0085,
	0x008A, 0x008F, 0x0095, 0x009A, 0x009F, 0x00A5, 0x00AB, 0x00B1, 0x00B7, 0x00BD, 0x00C4, 0x00CA, 0x00D1, 0x00D8, 0x00DF, 0x00E7,
	0x00EE, 0x00F6, 0x00FE, 0x0106, 0x010F, 0x0118, 0x0121, 0x012A, 0x0133, 0x013D, 0x0147, 0x0151, 0x015B, 0x0166, 0x0171, 0x017C,
	0x0187, 0x0193, 0x019F, 0x01AC, 0x01B8, 0x01C5, 0x01D3, 0x01E0, 0x01EE, 0x01FD, 0x020B, 0x021A, 0x022A, 0x0239, 0x024A, 0x025A,
	0x026B, 0x027C, 0x028E, 0x02A0, 0x02B3, 0x02C6, 0x02D9, 0x02ED, 0x0302, 0x0317, 0x032C, 0x0342, 0x0358, 0x036F, 0x0387, 0x039E
};

const unsigned long NoteTable[] = {
	0x000C265D, 0x000CDF51, 0x000DA344, 0x000E72DE, 0x000F4ED0, 0x001037D7, 0x00112EB8, 0x00123448, 0x00134965, 0x00146EFD, 0x0015A60A, 0x0016EF96, 0x00184CBB, 0x0019BEA2, 0x001B4689, 0x001CE5BD,
	0x001E9DA1, 0x00206FAE, 0x00225D71, 0x00246891, 0x002692CB, 0x0028DDFB, 0x002B4C15, 0x002DDF2D, 0x00309976, 0x00337D45, 0x00368D12, 0x0039CB7A, 0x003D3B43, 0x0040DF5C, 0x0044BAE3, 0x0048D122,
	0x004D2597, 0x0051BBF7, 0x0056982B, 0x005BBE5B, 0x006132ED, 0x0066FA8B, 0x006D1A24, 0x007396F4, 0x007A7686, 0x0081BEB9, 0x008975C6, 0x0091A244, 0x009A4B2F, 0x00A377EE, 0x00AD3056, 0x00B77CB6,
	0x00C265DB, 0x00CDF516, 0x00DA3449, 0x00E72DE9, 0x00F4ED0D, 0x01037D73, 0x0112EB8C, 0x01234489, 0x0134965F, 0x0146EFDC, 0x015A60AD, 0x016EF96D, 0x0184CBB6, 0x019BEA2D, 0x01B46892, 0x01CE5BD2,
	0x01E9DA1A, 0x0206FAE6, 0x0225D719, 0x02468912, 0x02692CBF, 0x028DDFB9, 0x02B4C15A, 0x02DDF2DB, 0x0309976D, 0x0337D45B, 0x0368D125, 0x039CB7A5, 0x03D3B434, 0x040DF5CC, 0x044BAE33, 0x048D1225,
	0x04D2597F, 0x051BBF72, 0x056982B5, 0x05BBE5B7, 0x06132EDB, 0x066FA8B6, 0x06D1A24A, 0x07396F4B, 0x07A76868, 0x081BEB99, 0x08975C67, 0x091A244A, 0x09A4B2FE, 0x0A377EE5, 0x0AD3056A, 0x0B77CB6E,
	0x0C265DB7, 0x0CDF516D, 0x0DA34494, 0x0E72DE96, 0x0F4ED0D1, 0x1037D732, 0x112EB8CE, 0x12344894, 0x134965FD, 0x146EFDCB, 0x15A60AD5, 0x16EF96DC, 0x184CBB6F, 0x19BEA2DB, 0x1B468928, 0x1CE5BD2C,
	0x1E9DA1A3, 0x206FAE64, 0x225D719D, 0x24689129, 0x2692CBFA, 0x28DDFB96, 0x2B4C15AA, 0x2DDF2DB9, 0x309976DF, 0x337D45B6, 0x368D1251, 0x39CB7A58, 0x3D3B4347, 0x40DF5CC9, 0x44BAE33A, 0x48D12252
};

const unsigned long Cmapping[] = {
	0x03F0D370, 0x04113BF3, 0x0432AF01, 0x04553521, 0x0478D733, 0x049D9E4A, 0x04C393DB, 0x04EAC196, 0x05133184, 0x053CEE01, 0x056801B9, 0x059477BD, 0x05C25B5F, 0x05F1B862, 0x06229AEB, 0x06550F7A,
	0x068922FE, 0x06BEE2C9, 0x06F65C99, 0x072F9E9E, 0x076AB787, 0x07A7B67C, 0x07E6AB09, 0x0827A55A, 0x086AB609, 0x08AFEE3C, 0x08F75FB8, 0x09411CBC, 0x098D3826, 0x09DBC57E, 0x0A2CD8D7, 0x0A8086EF,
	0x0AD6E539, 0x0B3009CF, 0x0B8C0B83, 0x0BEB01D8, 0x0C4D052E, 0x0CB22E85, 0x0D1A97CD, 0x0D865BD1, 0x0DF595FE, 0x0E6862FF, 0x0EDEE00D, 0x0F592B8A, 0x0FD764CC, 0x1059AC2A, 0x10E022D7, 0x116AEB71,
	0x11FA2945, 0x128E0135, 0x132698FD, 0x13C417AB, 0x1466A591, 0x150E6C51, 0x15BB96BC, 0x166E5159, 0x1726C9F1, 0x17E52F8A, 0x18A9B2E6, 0x19748653, 0x1A45DDDA, 0x1B1DEEE9, 0x1BFCF0FA, 0x1CE31D14,
	0x1DD0AE1F, 0x1EC5E0BA, 0x1FC2F404, 0x20C8285F, 0x21D5C0FA, 0x22EC02BB, 0x240B34E4, 0x2533A0E4, 0x266592AA, 0x27A1587E, 0x28E74328, 0x2A37A668, 0x2B92D7DC, 0x2CF93096, 0x2E6B0C74, 0x2FE8C9D7,
	0x3172CA8D, 0x330973D6, 0x34AD2D72, 0x365E6291, 0x381D82C2, 0x39EAFFC4, 0x3BC75008, 0x3DB2EDBC, 0x3FAE5631, 0x41BA0BB5, 0x43D69409, 0x4604799B, 0x48444B8B, 0x4A969D09, 0x4CFC05F3, 0x4F75237A,
	0x5202981D, 0x54A50A1A, 0x575D26E2, 0x5A2BA094, 0x5D112EEE, 0x600E8FF1, 0x6324873B, 0x6653DF4B, 0x699D68E2, 0x6D01FA5D, 0x7082737D, 0x741FB99F, 0x77DAB864, 0x7BB4656B, 0x7FADBC91, 0x83C7C1D7,
	0x8803829B, 0x8C62145D, 0x90E4955A, 0x958C2C93, 0x9A5A0B05, 0x9F4F6C4F, 0xA46D94D1, 0xA9B5D389, 0xAF298355, 0xB4CA09B5, 0xBA98D626, 0xC09765E7, 0xC6C742B7, 0xCD2A0055, 0xD3C142BD, 0xDA8EB7F0
};

const float chordtable[16][4] = {
	{ powf(2, 0.0f / 12.0f), 0, 0, 0 },
	{ powf(2, -0.1f / 12.0f), powf(2, 0.0f / 12.0f), powf(2, 0.1f / 12.0f), powf(2, 12.05f / 12.0f) }, // Fat Moog
	{ powf(2, -0.1f / 12.0f), powf(2, 0.0f / 12.0f), powf(2, 7.04f / 12.0f), powf(2, 0.1f / 12.0f) }, // Moog 5th
	{ powf(2, -0.18f / 12.0f), powf(2, -0.07f / 12.0f), powf(2, 0.07f / 12.0f), powf(2, 0.18f / 12.0f) }, // Guru Ps1
	{ powf(2, -0.3f / 12.0f), powf(2, -0.15f / 12.0f), powf(2, 0.15f / 12.0f), powf(2, 0.3f / 12.0f) }, // Guru Ps2
	{ powf(2, -0.1f / 12.0f), powf(2, 0.1f / 12.0f), powf(2, 12.02f / 12.0f), powf(2, 16.07f / 12.0f) }, // VengaBoyz:(

	{ powf(2, 0.0f / 12.0f), powf(2, 4.0f / 12.0f), powf(2, 7.0f / 12.0f), 0 }, // major 4-7
	{ powf(2, 0.0f / 12.0f), powf(2, 3.0f / 12.0f), powf(2, 7.0f / 12.0f), 0 }, // minor 3-7
	{ powf(2, 0.0f / 12.0f), powf(2, 4.0f / 12.0f), powf(2, 8.0f / 12.0f), 0 }, // minor 4-8
	{ powf(2, 0.0f / 12.0f), powf(2, 3.0f / 12.0f), powf(2, 8.0f / 12.0f), 0 }, // major 3-8
	{ powf(2, 0.0f / 12.0f), powf(2, 5.0f / 12.0f), powf(2, 8.0f / 12.0f), 0 }, // minor 5-8
	{ powf(2, 0.0f / 12.0f), powf(2, 5.0f / 12.0f), powf(2, 9.0f / 12.0f), 0 }, // major 5-9
	{ powf(2, 0.0f / 12.0f), powf(2, 5.0f / 12.0f), powf(2, 10.0f / 12.0f), powf(2, 1.0f / 12.0f) }, // minor 1-5-10
	{ powf(2, 0.0f / 12.0f), powf(2, 4.0f / 12.0f), powf(2, 7.0f / 12.0f), powf(2, 11.0f / 12.0f) }, // major 4-7-11
	{ powf(2, 0.0f / 12.0f), powf(2, 3.0f / 12.0f), powf(2, 7.0f / 12.0f), powf(2, 10.0f / 12.0f) }, // minor 3-7-10
	{ powf(2, 0.0f / 12.0f), powf(2, 3.0f / 12.0f), powf(2, 7.0f / 12.0f), powf(2, 9.0f / 12.0f) },  // minor 3-7-9

};

const int activechordnotes[16]
{
	1, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4
};

void P303::Init()
{
	Tab = saw[0];
	Transpose = 0;

	//	ParameterList[0].Name= "Wave";
	//	ParameterList[0].Description="Wave";
	ParameterList[0].Type = 30;
	ParameterList[0].MinValue = 0;
	ParameterList[0].MaxValue = 1;
	ParameterList[0].DefValue = 0;
	ParameterList[0].CurValue = 0;
	//	ParameterList[1].Name="Glide";
	//	ParameterList[1].Description="Glide";
	ParameterList[1].Type = 31;
	ParameterList[1].MinValue = 0;
	ParameterList[1].MaxValue = 127;
	ParameterList[1].DefValue = 0;
	ParameterList[1].CurValue = 0;
	//	ParameterList[2].Name="Cutoff";
	//	ParameterList[2].Description="Cutoff";
	ParameterList[2].Type = 32;
	ParameterList[2].MinValue = 0;
	ParameterList[2].MaxValue = 127;
	ParameterList[2].DefValue = 20;
	ParameterList[2].CurValue = 20;
	//	ParameterList[3].Name="Resonance";
	//	ParameterList[3].Description= "Resonance";
	ParameterList[3].Type = 33;
	ParameterList[3].MinValue = 0;
	ParameterList[3].MaxValue = 127;
	ParameterList[3].DefValue = 0;
	ParameterList[3].CurValue = 0;
	//ParameterList[4].Name= "EnvMod";
	//ParameterList[4].Description= "EnvMod";
	ParameterList[4].Type = 34;
	ParameterList[4].MinValue = 0;
	ParameterList[4].MaxValue = 127;
	ParameterList[4].DefValue = 74;
	ParameterList[4].CurValue = 74;
	//m_P6.Name= "Decay";
	//	m_P6.Description= "Decay";
	ParameterList[5].Type = 35;
	ParameterList[5].MinValue = 0;
	ParameterList[5].MaxValue = 127;
	ParameterList[5].DefValue = 64;
	ParameterList[5].CurValue = 64;


	ParameterList[6].Type = 36; //push
	ParameterList[6].MinValue = 0;
	ParameterList[6].MaxValue = 127;
	ParameterList[6].DefValue = 64;
	ParameterList[6].CurValue = 64;

	ParameterList[7].Type = 37; //pushtype
	ParameterList[7].MinValue = 0;
	ParameterList[7].MaxValue = 1;
	ParameterList[7].DefValue = 0;
	ParameterList[7].CurValue = 0;

	ParameterList[8].Type = 38; //fxsend
	ParameterList[8].MinValue = 0;
	ParameterList[8].MaxValue = 127;
	ParameterList[8].DefValue = 20;
	ParameterList[8].CurValue = 20;

	ParameterList[9].Type = 39; //fxparam
	ParameterList[9].MinValue = 0;
	ParameterList[9].MaxValue = 127;
	ParameterList[9].DefValue = 40;
	ParameterList[9].CurValue = 40;


	ParameterList[10].Type = 40; //fxtype
	ParameterList[10].MinValue = 0;
	ParameterList[10].MaxValue = 3;
	ParameterList[10].DefValue = 0;
	ParameterList[10].CurValue = 0;

	ParameterList[11].Type = 41; //accent
	ParameterList[11].MinValue = 0;
	ParameterList[11].MaxValue = 127;
	ParameterList[11].DefValue = 30;
	ParameterList[11].CurValue = 30;


	ParameterList[12].Type = 42; //filtertype
	ParameterList[12].MinValue = 0;
	ParameterList[12].MaxValue = 2;
	ParameterList[12].DefValue = 0;
	ParameterList[12].CurValue = 0;

	ParameterList[13].Type = 43; //chord
	ParameterList[13].MinValue = 0;
	ParameterList[13].MaxValue = 15;
	ParameterList[13].DefValue = 0;
	ParameterList[13].CurValue = 0;


	accented = false;
	AccentAMT = 0;

	samplecount = 0;

#if 0
	for (int i=0;i<128;i++)
	{
		DecayTime[i]=(unsigned long)(64*pow((500000*OVERSAMPLING)/64.0,pow((float)i/256.f,0.54)))>>(CONTROLBITS -1);
		NoteTable[i]=(unsigned long)((65536.f*65536.f) * (((440.f/OVERSAMPLING)*pow(2,(float)(i-69)/12))/44100.f));
		Cmapping[i]=(unsigned long)(20.0*zOSPowPlus(65535.0/20.0,(((float)i/2.f) + 62.f)/128.f)*65535);
	}
	for (int i=0;i<2048;i++)
	{
		//longpulse[i]=pulse[i]*256*16;
		//longsaw[i]=saw[i]*256*16;
	};  
#endif // 0

	playing = false;
	AEnvReset();
	FEnvReset();
	Note = 255;
	filtres = &filt.lo;
	filt.lo = 0;
	filt.mid = 0;
	filt.hi = 0;

	Tab = saw[0];
	Glide = 0;
	activechord = 0;
	C = Cmapping[ParameterList[Param_CUTOFF].DefValue];
	dC = 0;
	R = (unsigned short)((ParameterList[Param_RESONANCE].DefValue << 8) + (64 << 9));;
	sC = HI16(C);
	unsigned short Max = (0xffff - sC);
	unsigned long fR = R * Max;
	sR = 0xffff - HI16(fR);


	samplecount = 0;
	FenvAmt = 0;

	for (int i = 0; i < __GoldFishParamCount; i++)
	{
		Event(ParameterList[i].Type, ParameterList[i].DefValue, 0);
	}

	finertiacount = 0;
	pinertiacount = 0;

	P1 = 0;
	P2 = 0;
	P3 = 0;
	P4 = 0;
	dP = 0x80000;
	ddP = 0;

	ParamUpdate();
}


P303::P303()
{

}


P303::~P303()
{

}

#include "GoldfishSdkParameters.h"

inline void P303::ParamUpdate()
{
	if (finertiacount)
	{
		C += dC;
		finertiacount--;
	}
	if (pinertiacount)
	{
		dP += ddP;
		pinertiacount--;
	}

	signed long resC;
	unsigned long tempC = C;

	if (accented == true)
	{
		int cc = ParameterList[Param_CUTOFF].CurValue + AccentAMT;
		if (cc > 127)cc = 127;
		tempC = Cmapping[cc];
	}

	if (FenvAmt != 0)
	{
		resC = FEnvGet()*FenvAmt;
		if (FenvAmt > 0)
		{
			if ((unsigned long)resC > (0xDA8EB829 - tempC))
			{
				resC = 0xDA8EB829;
			}
			else
			{
				resC += tempC;
			};
		}
		else
		{
			if ((unsigned long)(-resC) > tempC - 66114413)
			{
				resC = 66114413;
			}
			else
			{
				resC += tempC;
			};
		};

	}
	else
	{
		resC = tempC;
	}
	sC = HI16(resC);

	unsigned short Max = (0xf200 - sC);
	unsigned long fR = R * Max;
	//	sR=0xffff-HI16(fR);
	sR = ~HI16(fR);

	signed char table = (Note + 20 + Transpose) / 12;
	if (table < 0) table = 0;

	if (table > 11) table = 11;

	if (ParameterList[0].CurValue > 0)
	{
		Tab = pulse[table];
	}
	else
	{
		Tab = saw[table];
	}
}


void P303::Event(WORD wEventType, int wParm1, int wParm2, int dwParm3/*=0*/, int dwParm4/*=0*/)
{
	switch (wEventType)
	{
	case ALL_NOTES_OFF:
	case RESET:
	{
		AEnvStop();
	}
	break;
	case NOTE_ON:

	{

		Note = wParm1;
		if (wParm2 > 100) accented = true; else accented = false;

		if (!playing)
		{
			AEnvStart();
			FEnvStart();
			Note = wParm1;
			int w = wParm1 + Transpose;
			if (w > 127) w = 127;
			if (w < 0) w = 0;
			dP = NoteTable[w];
			playing = true;

			samplecount = (CONTROLRES + 1);
			pinertiacount = 0;
		}
		else
		{

			if (Glide > 0 && dwParm3 == 0)
			{
				int w = wParm1 + Transpose;
				if (w > 127) w = 127;
				if (w < 0) w = 0;

				unsigned long tdp = NoteTable[w];
				if (tdp < dP)
				{
					ddP = (dP - tdp) / Glide;
					ddP = -ddP;
				}
				else
				{
					ddP = (tdp - dP) / Glide;
				}
				pinertiacount = Glide;
			}
			else
			{
				int w = wParm1 + Transpose;
				if (w > 127) w = 127;
				if (w < 0) w = 0;

				dP = NoteTable[w];
				pinertiacount = 0;
			}
			samplecount = 65;
			AEnvReStart();
			FEnvReStart();
			playing = true;
		}
		//wParm1	= Pitch		(0 - 127), 0 = C0, 127 = G10, 60 = C6
		//wParm2	= Velocity, (0 - 127)
		//dwParm3	= Length	(In Pulses)


	} break;
	case NOTE_OFF:
	{
		if (wParm1 == Note)
		{
			AEnvStop();
		}
		//wParm1	= Pitch		(0 - 127), 0 = C0, 127 = G10, 60 = C6
	} break;
	default:
	{
		int param = wEventType - 30;
		if (param < __GoldFishParamCount)
		{
			ParameterList[param].CurValue = wParm1;

			switch (param)
			{
			case 0:
			{
				if (wParm1 == 0)
				{
					Tab = saw[0];
				}
				else
				{
					Tab = pulse[0];
				}
			}
			break;
			case 1:
			{
				if (wParm1 == 0)
				{
					Glide = 0;
				}
				else
				{
					Glide = DecayTime[wParm1];
				}
			}
			break;
			case 2:
			{
				unsigned long nC = Cmapping[wParm1];
				if (nC > C)
				{
					dC = (nC - C) / 80;
					finertiacount = 80;
				}
				else
				{
					dC = (C - nC) / 80;
					dC = -dC;
					finertiacount = 80;
				}
			}
			break;
			case 3:
			{
				R = (unsigned short)((wParm1 << 8) + (64 << 9));
				unsigned short Max = (0xf200 - sC);
				unsigned long fR = R * Max;
				sR = ~HI16(fR);
			}
			break;
			case 4:
			{
				FenvAmt = ((int)wParm1 - 64)*(20000 / 64);
			}
			break;
			case 5:
			{
				FEnvDecay = DecayTime[wParm1];
			}
			break;
			case 6:
			{
				m_Dist->SetPush(wParm1);
			};
			break;
			case 7:
			{
				m_Dist->SetType(wParm1);

			};
			break;
			case 8:
			{
				// effectsend
				FXSend = wParm1 * (65535 / 127);

			};
			break;
			case 9:
			{
				// effectparam
				FXParam = wParm1;
			};
			break;
			case 10:
			{
				//effecttype
				delaymode = wParm1 % 4;

			};
			break;
			case 11:
			{
				AccentAMT = wParm1;
				//accent
			};
			break;
			case 12:
			{
				switch (wParm1)
				{
				default:
					filtres = &filt.lo;
					break;
				case 1: filtres = &filt.hi;
					break;
				case 2: filtres = &filt.mid;
					break;
				}
				//accent
			};
			break;
			case 13:
			{
				activechord = wParm1;
				//accent
			};
			break;
			}
		}
	}
	}
}

int32_t Distort(int32_t inp, uint32_t pre, uint32_t post)
{
	int32_t ii = inp;
	int32_t AA = pre;
	if (ii < 0)
	{
		if (ii > -32000)
		{
			if (ii > -AA)
			{

				//		f(x) = a + (x-a)/(1+((x-a)/(1-a))^2)
				ii = -ii;
				int32_t t = ((ii - AA) / (32768 - AA));
				t *= t;
				return -(AA + (ii - AA) / (32768 + (t >> 16)));
			}
			else
			{

				return ii;
			}
		}
		else
		{
			return -(AA + 32768) / 2;
		}
	}
	else
	{
		if (ii < 32000)
		{
			if (ii < AA)
			{

				//		f(x) = a + (x-a)/(1+((x-a)/(1-a))^2)
				int32_t t = ((ii - AA) / (32768 - AA));
				t *= t;
				return (AA + (ii - AA) / (32768 + t));
			}
			else
			{

				return ii;
			}
		}
		else
		{
			return (AA + 32768) / 2;
		}

	}

}

uint32_t P303::CalcSample()
{
	int32_t R = 0;
#if OVERSAMPLING > 1
	for (int j = 0; j < OVERSAMPLING; j++)
	{
#endif
		signed short const tMid = HI16(filt.mid);

		int const Shift = 21;
		//uint32_t IdX = P>>Shift;
		int32_t Fract = (P1 & ((1 << Shift) - 1)) >> (Shift - 16);

		// 21 bits of mantissa
		uint32_t	OscMask = (1 << (32 - Shift)) - 1;

		int32_t S1 = Tab[(P1>>Shift) & OscMask];
		int32_t S2 = Tab[((P1>>Shift) + 1) & OscMask];

		int32_t D = S2 - S1;

		uint32_t RR = S1 + ((D * Fract) >> 16);
		//	float Ref = (float)S1 + ((float)D*(float)Fract)/(float)(pow(2.0f,(float)16));

		filt.hi = (RR << 12) - filt.lo - sR * tMid;

		filt.mid += sC * HI16(filt.hi);
		filt.lo += sC * tMid;
		P1 += dP1;
#if OVERSAMPLING > 1

		R +=
#else
		R =
#endif

#ifdef BUZZ			
			(HI16(filt.lo) * AEnvGet());
#else				
			((HI16(*filtres) * AEnvGet())) >> (13 - 8);

		//R =  Distort(R, DistPreMult, DistPostMult);
		//			*pSamples++ = (HI16(filt.lo) * (AEnvGet()>>5));
#endif

#if OVERSAMPLING > 1

	}
	return  (int32_t)(R / OVERSAMPLING);
#else
		return (int32_t)(R);
#endif

}


	uint32_t P303::CalcSample3()
	{
		int32_t R = 0;
#if OVERSAMPLING > 1
		for (int j = 0; j<OVERSAMPLING; j++)
		{
#endif
			signed short const tMid = HI16(filt.mid);

			int const Shift = 21;
			uint32_t const OscMask = (1 << (32 - Shift)) - 1;

			//uint32_t IdX = P>>Shift;
			int32_t Fract_1 = (P1 & ((1 << Shift) - 1)) >> (Shift - 16);

			// 21 bits of mantissa

			int32_t S1_1 = Tab[(P1 >> Shift) & OscMask];
			int32_t S2_1 = Tab[((P1 >> Shift) + 1) & OscMask];

			int32_t D_1 = S2_1 - S1_1;

			uint32_t RR_1 = S1_1 + ((D_1 * Fract_1) >> 16);


			int32_t Fract_2 = (P2 & ((1 << Shift) - 1)) >> (Shift - 16);


			int32_t S1_2 = Tab[(P2 >> Shift) & OscMask];
			int32_t S2_2 = Tab[((P2 >> Shift) + 1) & OscMask];

			int32_t D_2 = S2_2 - S1_2;

			uint32_t RR_2 = S1_2 + ((D_2 * Fract_2) >> 16);


			int32_t Fract_3 = (P3 & ((1 << Shift) - 1)) >> (Shift - 16);


			int32_t S1_3 = Tab[(P3 >> Shift) & OscMask];
			int32_t S2_3 = Tab[((P3 >> Shift) + 1) & OscMask];

			int32_t D_3 = S2_3 - S1_3;

			uint32_t RR_3 = S1_3 + ((D_3 * Fract_3) >> 16);

			uint32_t RR = ((RR_1 + RR_2 + RR_3) >> 1);

			filt.hi = (RR << 12) - filt.lo - sR * tMid;

			filt.mid += sC * HI16(filt.hi);
			filt.lo += sC * tMid;
			P1 += dP1;
			P2 += dP2;
			P3 += dP3;
#if OVERSAMPLING > 1

			R +=
#else
			R =
#endif

#ifdef BUZZ			
				(HI16(filt.lo) * AEnvGet());
#else				
				((HI16(*filtres) * AEnvGet())) >> (13 - 8);

			//R =  Distort(R, DistPreMult, DistPostMult);
			//			*pSamples++ = (HI16(filt.lo) * (AEnvGet()>>5));
#endif

#if OVERSAMPLING > 1

		}
		return  (int32_t)(R / OVERSAMPLING);
#else
			return (int32_t)(R);
#endif

		}

		uint32_t P303::CalcSample4()
		{
			int32_t R = 0;
#if OVERSAMPLING > 1
			for (int j = 0; j<OVERSAMPLING; j++)
			{
#endif
				signed short const tMid = HI16(filt.mid);


				int const Shift = 21;
				uint32_t const OscMask = (1 << (32 - Shift)) - 1;

				//uint32_t IdX = P>>Shift;
				int32_t Fract_1 = (P1 & ((1 << Shift) - 1)) >> (Shift - 16);

				// 21 bits of mantissa

				int32_t S1_1 = Tab[(P1 >> Shift) & OscMask];
				int32_t S2_1 = Tab[((P1 >> Shift) + 1) & OscMask];

				int32_t D_1 = S2_1 - S1_1;

				uint32_t RR_1 = S1_1 + ((D_1 * Fract_1) >> 16);


				int32_t Fract_2 = (P2 & ((1 << Shift) - 1)) >> (Shift - 16);


				int32_t S1_2 = Tab[(P2 >> Shift) & OscMask];
				int32_t S2_2 = Tab[((P2 >> Shift) + 1) & OscMask];

				int32_t D_2 = S2_2 - S1_2;

				uint32_t RR_2 = S1_2 + ((D_2 * Fract_2) >> 16);


				int32_t Fract_3 = (P3 & ((1 << Shift) - 1)) >> (Shift - 16);


				int32_t S1_3 = Tab[(P3 >> Shift) & OscMask];
				int32_t S2_3 = Tab[((P3 >> Shift) + 1) & OscMask];

				int32_t D_3 = S2_3 - S1_3;

				uint32_t RR_3 = S1_3 + ((D_3 * Fract_3) >> 16);

				int32_t Fract_4 = (P4 & ((1 << Shift) - 1)) >> (Shift - 16);


				int32_t S1_4 = Tab[(P4 >> Shift) & OscMask];
				int32_t S2_4 = Tab[((P4 >> Shift) + 1) & OscMask];

				int32_t D_4 = S2_4 - S1_4;

				uint32_t RR_4 = S1_4 + ((D_4 * Fract_4) >> 16);

				uint32_t RR = (RR_1 + RR_2 + RR_3 + RR_4) >> 2;



				filt.hi = (RR << 12) - filt.lo - sR * tMid;

				filt.mid += sC * HI16(filt.hi);
				filt.lo += sC * tMid;
				P1 += dP1;
				P2 += dP2;
				P3 += dP3;
				P4 += dP4;
#if OVERSAMPLING > 1

				R +=
#else
				R =
#endif

#ifdef BUZZ			
					(HI16(filt.lo) * AEnvGet());
#else				
					((HI16(*filtres) * AEnvGet())) >> (13 - 8);

				//R =  Distort(R, DistPreMult, DistPostMult);
				//			*pSamples++ = (HI16(filt.lo) * (AEnvGet()>>5));
#endif

#if OVERSAMPLING > 1

		}
			return  (int32_t)(R / OVERSAMPLING);
#else
				return (int32_t)(R);
#endif

			}


			int32_t buffer[32];

			void P303::DelayProcess(INTERNAL_RES *pSamples, int nSamples)
			{

				switch (delaymode)
				{
				case 0:
				{

					uint32_t amt1 = (unsigned short)FXSend;
					uint32_t iamt1 = 65535 - amt1;
					uint32_t  feedback = (int)(65535 * 0.8);
					uint32_t  Length = (DELAYLEN / 127) * FXParam;

					for (int i = 0; i < nSamples; i++)
					{
						int32_t inp = *pSamples;
						uint32_t readpos = (writepos - Length + DELAYLEN) % DELAYLEN;

						int16_t Res = delay[0][readpos];

						int32_t w = ((((inp * 50000) + (feedback * Res)) >> 16));;



						delay[0][writepos] = (int16_t)w;
						uint32_t t = ((inp * iamt1 + Res * amt1) >> 16);
						*pSamples++ = t;
						writepos = (writepos + 1) % DELAYLEN;
					}
					break;
				}
				case 1:
				{
					uint32_t amt1 = (unsigned short)FXSend;
					uint32_t iamt1 = 65535 - amt1;
					//uint32_t  feedback = 0;// (int)(65535 * 0.8);
					//uint32_t  Length = (DELAYLEN / 127) * FXParam;
					uint32_t noteadd = NoteTable[FXParam] / 64;
					for (int i = 0; i < nSamples; i++)
					{
						uint32_t inp = *pSamples;
						uint32_t readpos = (writepos - (44 * 10) - ((saw[11][ChorusPhase >> 21]) >> 7) + DELAYLEN * 2) % DELAYLEN;
						ChorusPhase += noteadd;

						int16_t Res = delay[0][readpos];

						delay[0][writepos] = inp;// (((inp << 16) + (feedback * Res)) >> 16);
						uint32_t t = ((inp * iamt1 + Res * amt1) >> 16);
						*pSamples++ = t;
						writepos = (writepos + 1) % DELAYLEN;
					}
					break;
				}
				case 2:
				{
					uint32_t amt1 = (unsigned short)FXSend;
					uint32_t iamt1 = 65535 - amt1;
					//uint32_t  feedback =  (int)(65535 * 0.5);
					//uint32_t  Length = (DELAYLEN / 127) * FXParam;
					uint32_t noteadd = NoteTable[FXParam];
					for (int i = 0; i < nSamples; i++)
					{
						uint32_t inp = *pSamples;
						uint32_t readpos = (writepos - (44 * 10) - ((saw[11][ChorusPhase >> 21]) >> 8) + DELAYLEN * 2) % DELAYLEN;
						ChorusPhase += noteadd;

						int16_t Res = delay[0][readpos];

						delay[0][writepos] = inp;// (((inp << 16) + (feedback * Res)) >> 16);
						uint32_t t = ((inp * iamt1 + Res * amt1) >> 16);
						*pSamples++ = t;
						writepos = (writepos + 1) % DELAYLEN;
					}
					break;
				}
				break;
				case 3:
					break;
				}
	}

			bool P303::Render(INTERNAL_RES* pSamples, int nSamples, PlatinumClip *dist)
			{
				//bool active = true;
				int leftover = nSamples;
				while (leftover > 0)
				{
					nSamples = (32 < leftover) ? 32 : leftover;
					leftover -= nSamples;

					int32_t *b = buffer;
					samplecount += nSamples;
					while (samplecount > (CONTROLRES - 1))
					{
						ParamUpdate();
						samplecount -= CONTROLRES;
					}
					if (true)
					{
						int nn = nSamples;
						INTERNAL_RES *tbuf = pSamples;
						switch (activechordnotes[activechord])
						{
						case 1:
							dP1 = dP;
							while (nn--)
							{
								*b++ = CalcSample();
							}break;
						case 3:
							dP1 = (chordtable[activechord][0] * dP);;
							dP2 = (chordtable[activechord][1] * dP);
							dP3 = (chordtable[activechord][2] * dP);
							while (nn--)
							{
								*b++ = CalcSample3();
							}break;
						case 4:
							dP1 = (chordtable[activechord][0] * dP);;
							dP2 = (chordtable[activechord][1] * dP);
							dP3 = (chordtable[activechord][2] * dP);
							dP4 = (chordtable[activechord][3] * dP);
							while (nn--)
							{
								*b++ = CalcSample4();
							}break;
						}

						dist->IntProcess(&buffer[0], nSamples);
						for (int i = 0; i < nSamples; i++)
						{
							*pSamples++ = (INTERNAL_RES)(buffer[i] >> 9);
						}
						if (FXSend > 0) DelayProcess(tbuf, nSamples);
						if (!AEnvState){ playing = false; }


					}
					else
					{
						//	active = false;
					}
				}
				return true;
			}


			bool P303::RenderAdd(INTERNAL_RES* pSamples, int nSamples, PlatinumClip *dist)
			{
				samplecount += nSamples;
				while (samplecount > (CONTROLRES - 1))
				{
					ParamUpdate();
					samplecount -= CONTROLRES;
				}
				if (playing)
				{

					while (nSamples--)
					{
						*pSamples++ += CalcSample();
					}
					if (!AEnvState){ playing = false; }
					return true;
				}
				else
				{
					return false;
				}
				return true;
			}


#endif
