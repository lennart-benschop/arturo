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

#define KBD_REPEAT_START 	(90)  												// Time for first repeat
#define KBD_REPEAT_AFTER  	(12)  												// Time for subsequent repeats

#ifndef HANDLE_USB_KBD_MESSAGES

void KBDReceiveEvent(uint8_t isDown,uint8_t keyCode,uint8_t modifiers);
void KBDCheckTimer(void);
uint8_t KBDGetModifiers(void);
void KBDInsertQueue(uint8_t ascii);
bool KBDIsKeyAvailable(void);
uint8_t KBDGetKey(void);

#define USBKeyboardEvent(x,y,z) KBDReceiveEvent(x,y,z)

#else

#define USBKeyboardEvent(x,y,z) CONWriteString("%d %d %d\r",x,y,z)

#endif

#endif