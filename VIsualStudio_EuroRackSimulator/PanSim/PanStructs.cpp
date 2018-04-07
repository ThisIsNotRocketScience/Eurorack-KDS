#include "PanHeader.h"


#define LEDBUTTON(name,x,y,min,max) +1
const int ledbuttoncount = 0
#include "PanControls.h"
;
#undef LEDBUTTON

#define LED(name,x,y,min,max) +1
const int ledcount = 0
#include "PanControls.h"
;
#undef LED


aLedButton Buttons[ledbuttoncount] = {
#define LEDBUTTON(iname,ix,iy) {iname,ix, iy},
#include "PanControls.h"
#undef LEDBUTTON

};

Led Leds[ledcount] = {
#define LED(iname,ix,iy) {iname,ix, iy},
#include "PanControls.h"
#undef LED

};

Knob Knobs[knobcount] = {
#define KNOB(iname,ix,iy,imin,imax) {iname,ix, iy},
#include "PanControls.h"
#undef KNOB

};
