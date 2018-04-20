#include "PanHeader.h"




aLedButton Buttons[__LEDBUTTON_COUNT] = {
#define LEDBUTTON(iname,ix,iy) {#iname,ix, iy,ledbutton_##iname},
#include "PanControls.h"
#undef LEDBUTTON

};

Led Leds[__LED_COUNT] = {
#define LED(iname,ix,iy) {#iname,ix, iy, led_##iname},
#include "PanControls.h"
#undef LED

};

Knob Knobs[__KNOB_COUNT] = {
#define KNOB(iname,ix,iy,imin,imax) {#iname,ix, iy,knob_##iname},
#include "PanControls.h"
#undef KNOB

};

#define SCREEN(iname,ix,iy,iw,ih) {ix, iy,iw,ih}
Screen TheScreen =
#include "PanControls.h"
;
#undef SCREEN


