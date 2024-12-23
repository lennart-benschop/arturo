// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sound.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      19th December 2024
//      Reviewed :  No
//      Purpose :   Sound interface headers (internal)
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _SOUNDI_H
#define _SOUNDI_H

void SNDInitialise(bool _combineChannels);
//
//		Sound hardware setup.
//
#if PICO_HARDWARE==PICO_HW_RP2040PC
#define AUDIO_HARDWARE_CHANNELS		(2)
#define AUDIO_PIN_L 				(20) 
#define AUDIO_PIN_R 				(21) 
#elif PICO_HARDWARE==PIC_HW_NEO602
#define AUDIO_HARDWARE_CHANNELS		(1)
#define AUDIO_PIN_L 				(20) 
#define AUDIO_PIN_R 				(-1)
#else
#error("Unsupported hardware")
#endif

#define SAMPLE_DIVIDER (32)                 // Divider, affects the interrupts / second of the PWM sample output. More samples are better but slower.

#endif
