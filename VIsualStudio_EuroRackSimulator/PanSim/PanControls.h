
#ifndef KNOB
	#define KNOB(name,x,y,mintext,maxtext)
#endif
#ifndef LEDBUTTON
	#define LEDBUTTON(name,x,y)
#endif
#ifndef LED
	#define LED(name,x,y)
#endif

KNOB("Master Tune", 1, 1,"","")


LEDBUTTON("VCO1", 2, 1)
KNOB("Pitch 1", 3, 1, "-2 oct", "+2 oct")
KNOB("Pulsewidth 1", 4, 1, "", "")
LEDBUTTON("VCO4", 5, 1)
KNOB("Pitch 4", 6, 1, "min", "max")
KNOB("VCF1 Filter Frequency", 7, 1, "min", "max")
KNOB("VCF1 Resonance", 8, 1, "0", "")
LED("VCF1 O/L", 9, 1)
KNOB("VCF2 Spectrum mod", 10, 1, "0", "")
KNOB("VCF2 Feedback", 11, 1, "0", "")
KNOB("VCF2 Resonance", 12, 1, "0", "")
LED("VCF2 O/L", 13, 1)
KNOB("Low", 14, 1, "min", "max")
KNOB("Mid 1", 15, 1, "min", "max")
KNOB("Mid 2", 16, 1, "min", "max")
KNOB("High", 17, 1, "min", "max")

LEDBUTTON("M/S", 18, 1)
KNOB("Cleanfeed", 19, 1, "L", "R")
LEDBUTTON("M/S", 20, 1)
KNOB("VCF1", 21, 1, "L", "R")
LEDBUTTON("M/S", 22, 1)
KNOB("VCF2", 23, 1, "L", "R")


KNOB("FM 1->2", 1, 2, "0", "")
LEDBUTTON("VCO2", 2, 2)
KNOB("Pitch2", 3, 2, "-1/3d", "+1/3d")
KNOB("Pulsewidth 2", 4, 2, "", "")
LEDBUTTON("VCO5", 5, 2)
KNOB("Pitch5", 6, 2, "0", "+3 oct")


KNOB("VCF1 Spectrum mod", 8, 2.2, "0", "")
KNOB("VCF1 Feedback", 9, 2.2, "0", "")

LEDBUTTON("LowButton", 2, 1)

LEDBUTTON("Mid1Button", 2, 1)

LEDBUTTON("Mid2BUtton", 2, 1)

LEDBUTTON("HighButton", 2, 1)


#undef LEDBUTTON
#undef KNOB
#undef LED
