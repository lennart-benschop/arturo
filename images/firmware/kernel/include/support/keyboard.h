// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		keyboard.h
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		18th December 2024
//		Reviewed :	No
//		Purpose :	Keyboard full driver, decoding, autorepeat.
//
// ***************************************************************************************
// ***************************************************************************************

#pragma once

#define KBD_REPEAT_START 	(90)  												// Time for first repeat (100ms units)
#define KBD_REPEAT_AFTER  	(12)  												// Time for subsequent repeats (100ms units)

void KBDReceiveEvent(uint8_t isDown,uint8_t keyCode,uint8_t modifiers);
void KBDCheckTimer(void);
int KBDGetModifiers(void);
void KBDInsertQueue(int ascii);
int KBDIsKeyAvailable(void);
uint8_t *KBDGetStateArray(void);
int KBDGetKey(void);
int KBDEscapePressed(int resetEscape);
