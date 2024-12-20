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

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#define KBD_REPEAT_START 	(90)  												// Time for first repeat (100ms units)
#define KBD_REPEAT_AFTER  	(12)  												// Time for subsequent repeats (100ms units)

void KBDReceiveEvent(uint8_t isDown,uint8_t keyCode,uint8_t modifiers);
void KBDCheckTimer(void);
uint8_t KBDGetModifiers(void);
void KBDInsertQueue(uint8_t ascii);
bool KBDIsKeyAvailable(void);
uint8_t KBDGetKey(void);

//
//		If this constant is set to non-zero, the full keyboard decoding is used. Otherwise
//		the user can handle the Keyboard events from the usb driver themselves.
//
#if  	ARTURO_PROCESS_KEYS != 0
#define USBKeyboardEvent(x,y,z) KBDReceiveEvent(x,y,z)
#endif

#endif