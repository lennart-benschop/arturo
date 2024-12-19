// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		control_codes.h
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		18th December 2024
//		Reviewed :	No
//		Purpose :	Control code values.
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _CONTROL_CODES_H
#define _CONTROL_CODES_H

#define CONTROL(c) 	((c) & 0x1F)

#define CC_LEFT 	CONTROL('A')
#define CC_RIGHT 	CONTROL('D')
#define CC_INSERT 	CONTROL('E')
#define CC_PAGEDOWN	CONTROL('F')
#define CC_END 		CONTROL('G')
#define CC_BACKSPACE CONTROL('H')
#define CC_TAB  	CONTROL('I')
#define CC_LF  		CONTROL('J')
#define CC_CLS  	CONTROL('L')
#define CC_ENTER 	CONTROL('M')
#define CC_PAGEUP	CONTROL('R')
#define CC_DOWN 	CONTROL('S')
#define CC_HOME 	CONTROL('T')
#define CC_UP  		CONTROL('W')
#define CC_DELETE 	CONTROL('Z')
#define CC_ESC 		CONTROL('[')

#endif