#pragma once
#ifndef SYNTONPAN
#define SYNTONPAN
#include <string>

#define KNOB(name,x,y,min,max) +1
const int knobcount = 0
#include "PanControls.h"
;
#undef KNOB


typedef struct _param_t
{	
	uint16_t value;

	uint8_t lfo_group;
	uint16_t lfo_speed;
	int16_t lfo_depth;
	int16_t lfo_shape;

	uint8_t adsr_group;
	uint16_t adsr_a;
	uint16_t adsr_d;
	uint16_t adsr_s;
	uint16_t adsr_r;
	int16_t adsr_depth;

	uint8_t ad_group;
	uint16_t ad_a;
	uint16_t ad_d;
	int16_t ad_depth;

	uint8_t xgroup;
	int16_t x;
	uint8_t ygroup;
	int16_t y;
	uint8_t zgroup;
	int16_t z;

	uint8_t zprimegroup;
	int16_t zprime;
	
	uint8_t notegroup;
	int16_t note;
	
	uint8_t velgroup;
	int16_t vel;

	uint8_t aftertouchgroup;
	int16_t aftertouch;
} param_t;


enum paramorderenum{
	 value,

	lfo_group,
	lfo_speed,
	lfo_depth,
	lfo_shape,

	adsr_group,
	adsr_a,
	adsr_d,
	adsr_s,
	adsr_r,
	adsr_depth,

	ad_group,
	ad_a,
	ad_d,
	ad_depth,

	xgroup,
	x,
	ygroup,
	y,
	zgroup,
	z,

	zprimegroup,
	zprime,

	notegroup,
	note,

	velgroup,
	vel,

	aftertouchgroup,
	aftertouch
};

class aLedButton
{
public:

	char *name;
	float x;
	float y;
	int id;
	bool value;
};

extern aLedButton Buttons[];

extern const int ledbuttoncount;

class Led
{
public:

	char* name;
	float x;
	float y;
	int id;
	bool value;
};



class Knob
{
public:

	char *name;
	float x;
	float y;
	int id;
	float value;
};

class Screen
{
public:
	float x;
	float y;
	float width;
	float height;
};

extern Knob Knobs[];
extern Led Leds[];
extern Screen TheScreen;



typedef struct PanScanner_t
{
	
} PanScanner_t;
typedef enum
{
	Page_BOOT,
	Page_HOME,
	Page_MidiSettings,
	
	Page_Source_LFO,
	Page_Source_ADSR,
	Page_Source_AR,
	Page_Source_KEYBOARD_VELOCITY,
	Page_Source_KEYBOARD_GATE,
	Page_Source_KEYBOARD_TRIGGER,
	Page_Source_KEYBOARD_AFTERTOUCH,
	Page_Source_PADLEFT_A,
	Page_Source_PADRIGHT_A,

	Page_Source_XYZ_X,
	Page_Source_XYZ_Y,
	Page_Source_XYZ_Z,
} Pages;

extern void GUI_OpenPage(Pages Page);
extern void GUI_Home(Pages Page);

extern void GUI_Enc1(int Delta);
extern void GUI_Enc1Press();
extern void GUI_Enc2(int Delta);
extern void GUI_Enc2Press();

extern void GUI_Enter();
extern void GUI_Cancel();

#define KNOB(name,x,y,min,max)  knob_##name,
enum KnobEnum
{
#include "PanControls.h"
	__KNOB_COUNT
};
#undef KNOB

#define LEDBUTTON(name,x,y)  ledbutton_##name,
enum LedButtonEnum
{
#include "PanControls.h"
	__LEDBUTTON_COUNT
};

#undef LEDBUTTON

#define LED(name,x,y)  led_##name,
enum LedEnum
{
#include "PanControls.h"
	__LED_COUNT
};

#undef LEDBUTTON
typedef struct PanGui_t
{
	
} PanGui_t;

extern void KnobChanged(int ID, int value);
extern void ButtonPressed(int ID, int value);

extern void RenderScreen();
extern void EncoderTurn(int id, int delta);

#endif