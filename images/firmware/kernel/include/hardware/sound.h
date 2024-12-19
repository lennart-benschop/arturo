// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sound.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      19th December 2024
//      Reviewed :  No
//      Purpose :   PWM Audio support header
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _SOUND_H
#define _SOUND_H

int SNDGetSampleFrequency(void);
void SNDInitialise(void);
int8_t SNDGetNextSample(void);

#define AUDIO_PIN   (20) 
#define SAMPLE_DIVIDER (32)                 // Divider, affects the interrupts / second of the PWM sample output

#endif
