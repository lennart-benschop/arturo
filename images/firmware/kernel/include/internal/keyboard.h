// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		keyboard.h
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		23rd December 2024
//		Reviewed :	No
//		Purpose :	Keyboard full driver, decoding, autorepeat (internal)
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _KEYBOARDI_H
#define _KEYBOARDI_H

//
//		If this constant is set to non-zero, the full keyboard decoding is used. Otherwise
//		the user can handle the Keyboard events from the usb driver themselves.
//
#if  	ARTURO_PROCESS_KEYS != 0
#define USBKeyboardEvent(x,y,z) KBDReceiveEvent(x,y,z)
#endif

#endif