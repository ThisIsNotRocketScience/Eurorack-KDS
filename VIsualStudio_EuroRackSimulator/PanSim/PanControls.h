
#ifndef KNOB
	#define KNOB(name,x,y,mintext,maxtext)
#endif
#ifndef LEDBUTTON
	#define LEDBUTTON(name,x,y)
#endif
#ifndef LED
	#define LED(name,x,y)
#endif


KNOB("Master Tune", 1, 1, "", "")
LEDBUTTON("VCO1", 2, 1)
KNOB("VCO1 Pitch", 3, 1, "-2 oct", "+2 oct")
KNOB("VCO1 PW", 4, 1, "", "")
LEDBUTTON("VCO4", 5, 1)
KNOB("VCO4 Pitch", 6, 1, "min", "max")

KNOB("FM 1->2", 1, 2, "0", "")
LEDBUTTON("VCO2", 2, 2)
KNOB("VCO2 Pitch", 3, 2, "-1/3d", "+1/3d")
KNOB("VCO2 PW", 4, 2, "", "")
LEDBUTTON("VCO5", 5, 2)
KNOB("VCO5 Pitch", 6, 2, "0", "+3 oct")

KNOB("FM 2->3", 1, 3, "0", "")
LEDBUTTON("VCO3", 2, 3)
KNOB("VCO3 Pitch", 3, 3, "-1/3d", "+1/3d")
KNOB("VCO3 PW", 4, 3, "", "")
LEDBUTTON("VCO6", 5, 3)
KNOB("VCO6 Pitch", 6, 3, "0", "+3 oct")

LEDBUTTON("VCO7", 5, 4)
KNOB("VCO7 Pitch", 6, 4, "0", "+3 oct")


LEDBUTTON("Interval spread", 2, 4)

#define VCF1X 8
#define VCF1Y 1.5

KNOB("VCF1 Resonance", VCF1X, VCF1Y, "min", "max")
LEDBUTTON("VCF1 ResonanceBut", VCF1X - 0.2, VCF1Y + 0.5)
LED("VCF1 O/L", VCF1X + 0.5, VCF1Y - 0.2)
KNOB("VCF1 Spectrum Mod", VCF1X + 1, VCF1Y, "0", "")
LEDBUTTON("VCF1 Spectrum ModBut", VCF1X + 1.5, VCF1Y + 0.5)

KNOB("VCF1 Frequency", VCF1X + 0.5, VCF1Y + 1, "min", "max")
LEDBUTTON("VCF1 FrequencyBut", VCF1X, VCF1Y + 1.5)

#define VCF2X 11
#define VCF2Y 1.1


KNOB("VCF2 Spectrum mod", VCF2X, VCF2Y, "0", "")
KNOB("VCF2 Feed Back", VCF2X + 1, VCF2Y, "0", "")
KNOB("VCF2 Resonance", VCF2X + 2, VCF2Y, "0", "")
LED("VCF2 O/L", VCF2X + 2.5, VCF2Y - 0.2)

LEDBUTTON("VCF2 SPM", VCF2X, VCF2Y + 0.7)
LEDBUTTON("VCF2 F/B", VCF2X, VCF2Y + 1.2)
LEDBUTTON("VCF2 RES", VCF2X, VCF2Y + 1.7)


#define BANKX 14
#define BANKY 1
KNOB("Low", BANKX, BANKY, "min", "max")
KNOB("Mid 1", BANKX + 1, BANKY, "min", "max")
KNOB("Mid 2", BANKX + 2, BANKY, "min", "max")
KNOB("High", BANKX + 3, BANKY, "min", "max")

LEDBUTTON("LowButton", BANKX, BANKY + 1)
LEDBUTTON("Mid1Button", BANKX+1, BANKY + 1)
LEDBUTTON("Mid2BUtton", BANKX+2, BANKY + 1)
LEDBUTTON("HighButton", BANKX+3, BANKY + 1)


KNOB("PB1", BANKX, BANKY+2, "min", "max")
KNOB("PB2", BANKX + 1, BANKY+2, "min", "max")
KNOB("PB3", BANKX + 2, BANKY+2, "min", "max")
KNOB("PB4", BANKX + 3, BANKY+2, "min", "max")

LEDBUTTON("PB1but", BANKX, BANKY + 3)
LEDBUTTON("PB2but", BANKX + 1, BANKY + 3)
LEDBUTTON("PB3but", BANKX + 2, BANKY + 3)
LEDBUTTON("PB4but", BANKX + 3, BANKY + 3)

#define AMPX 19
#define AMPY 1.5

LEDBUTTON("Cleanfeed M/S", AMPX, AMPY+0.5)
KNOB("Cleanfeed", AMPX+1, AMPY, "L", "R")
LEDBUTTON("VCF1 M/S", AMPX+2, AMPY+0.5)
KNOB("VCF1", AMPX+3, AMPY, "L", "R")
LEDBUTTON("VCF2 M/S", AMPX+4, AMPY+0.5)
KNOB("VCF2", AMPX+5, AMPY, "L", "R")

LEDBUTTON("Cleanfeed VCA", AMPX, AMPY + 1.5)
KNOB("Cleanfeed Lvl", AMPX + 1, AMPY+1, "L", "R")
LEDBUTTON("VCF1 VCA", AMPX + 2, AMPY + 1.5)
KNOB("VCF1 Lvl", AMPX + 3, AMPY+1, "L", "R")
LEDBUTTON("VCF2 VCA", AMPX + 4, AMPY + 1.5)
KNOB("VCF2 lvl", AMPX + 5, AMPY+1, "L", "R")

LEDBUTTON("Cleanfeed FX", AMPX + 1, AMPY + 2)
LEDBUTTON("VCF1 FX", AMPX + 3, AMPY + 2)
LEDBUTTON("VCF2 FX", AMPX + 5, AMPY + 2)







#undef LEDBUTTON
#undef KNOB
#undef LED
