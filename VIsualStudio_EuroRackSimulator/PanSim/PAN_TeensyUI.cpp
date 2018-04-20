#include "PanHeader.h"

void KnobChanged(int ID, int value)
{
	printf("knob %s: %f\n", Knobs[ID].name, Knobs[ID].value);

}

void ButtonPressed(int ID, int value)
{
	printf("button %s: %d\n", Buttons[ID].name, Buttons[ID].value ? 1 : 0);



	/*
modes:

	press a target, get detail menu for just target

	
	press a source once
		- all bound targets light up
		- screen menu 
			- source details + current modulation?
			- target amounts	
		- select all sources that apply (with submenu)
	
	

	*/
}

