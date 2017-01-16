#ifndef TUESDAY_ALGO_H
#define TUESDAY_ALGO_H

#include <stdint.h>
#include "Tuesday.h"

struct ScaledNote
{
	int32_t oct;
	int32_t note;
};

#define NOTE(aoct, anote) { SN.note = anote;SN.oct = aoct;};
int ScaleToNote(struct ScaledNote *SN, struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S);


typedef void(*GenFuncPtr)(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output);
typedef void(*InitFuncPtr)(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output);
typedef void(*PatternInitFuncPtr)(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_PatternContainer *PT);

void NoInit(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *Output);
void NoPattern(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output);
void NoPatternInit(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_PatternContainer *PT);

void Pattern_Rotate(struct Tuesday_PatternContainer *T, int first, int length, int rotate);
void Pattern_Reverse(struct Tuesday_PatternContainer *T, int first, int length);
void Pattern_Transpose(struct Tuesday_PatternContainer *T, int first, int length, int transpose);

int Tuesday_Rand(struct Tuesday_RandomGen *R);

struct PatternFunctions
{
	GenFuncPtr Gen;
	InitFuncPtr Init;
	PatternInitFuncPtr PatternInit;
	uint8_t Dither;
};


void Algo_Saiko_Lead_Gen(struct Tuesday_PatternGen *T, struct Tuesday_Params *P, struct Tuesday_Settings *S, struct Tuesday_RandomGen *R, struct Tuesday_PatternFuncSpecific *PS, int I, struct Tuesday_Tick *Output);



#endif