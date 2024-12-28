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
uint8_t KBDGetModifiers(void);
void KBDInsertQueue(uint8_t ascii);
bool KBDIsKeyAvailable(void);
uint8_t KBDGetKey(void);
bool KBDEscapePressed(bool resetEscape);
