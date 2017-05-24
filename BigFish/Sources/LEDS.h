/*
 * LEDS.h
 *
 *  Created on: 6 jan. 2017
 *      Author: Stijn
 */

#ifndef SOURCES_LEDS_H_
#define SOURCES_LEDS_H_

void LEDS_InitHardware();
void LEDS_Update(uint8_t *val, int count, uint8_t *gates, int gatecount);

#endif /* SOURCES_LEDS_H_ */
