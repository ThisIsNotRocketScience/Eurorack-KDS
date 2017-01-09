#include "Reader.h"

void DecoderUpdate(int32_t NewVal);
void DecoderInit();
void SuccesLedOn();
void SuccesLedOff();
void ErrorLedOn();
void ErrorLedOff();

int GetErrorLed();
int GetSuccesLed();
void GUIErrorState();
void GUISuccesState();
void GUIProgress(uint8_t progress) ; // 255 = 99.999%
void GUIComplete();
void GUIInit();
