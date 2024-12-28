// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      kbd_codes.h
//      Authors :   MightyPork
//                  Paul Robson (paul@robsons.org.uk)
//      Date :      18th December 2024
//      Reviewed :  No
//      Purpose :   Keyboard codes we use. These are the same as the HID codes.
//
// ***************************************************************************************
// ***************************************************************************************

#pragma once
//
//		Bits in the first byte of the data packet.
//
#define _KEY_MOD_LCTRL  0x01
#define _KEY_MOD_LSHIFT 0x02
#define _KEY_LEFT_ALT   0x04
#define _KEY_MOD_RCTRL  0x10
#define _KEY_MOD_RSHIFT 0x20
#define _KEY_RIGHT_ALT 	0x40
//
//		Actual mask bits.
//
#define KEY_NOSHIFT 	(0)
#define KEY_SHIFT 		(_KEY_MOD_LSHIFT|_KEY_MOD_RSHIFT)
#define KEY_CONTROL 	(_KEY_MOD_LCTRL|_KEY_MOD_RCTRL)
#define KEY_ALT  		(_KEY_LEFT_ALT)
#define KEY_ALTGR 		(_KEY_RIGHT_ALT)
//
//		Key sequence to Reboot.
//
#define REBOOT_KEYS 	(_KEY_MOD_LCTRL|_KEY_LEFT_ALT|_KEY_RIGHT_ALT)
//
//		Keys A-Z are in order
//
#define KEY_A 0x04 		    
//
//		Number keys are in the tweaked order 1-9,0 matching the keyboard.
// 		Two sets, top row and number keypad.
//
#define KEY_1 0x1e 		    
#define KEY_KP1 0x59  			// We don't use these 10, but the USB routine can map them onto $1E...
//
//		Standard keys
//
#define KEY_ENTER 0x28 	       	// Keyboard Return (ENTER) 			M / 13
#define KEY_ESC 0x29 		   	// Keyboard ESCAPE					[ / 27
#define KEY_BACKSPACE 0x2a 		// Keyboard DELETE (Backspace) 		127
#define KEY_TAB 0x2b 			// Keyboard Tab 					I / 9

#define KEY_SPACE 0x2c 			// Keyboard Spacebar 				
#define KEY_MINUS 0x2d 			// Keyboard - and _ 
#define KEY_EQUAL 0x2e 			// Keyboard = and +
#define KEY_LEFTBRACE 0x2f 		// Keyboard [ and {
#define KEY_RIGHTBRACE 0x30 	// Keyboard ] and }
#define KEY_BACKSLASH 0x31 		// Keyboard \ and |
#define KEY_HASHTILDE 0x32 		// Keyboard Non-US # and ~
#define KEY_SEMICOLON 0x33 		// Keyboard ; and :
#define KEY_APOSTROPHE 0x34 	// Keyboard ' and "
#define KEY_GRAVE 0x35 			// Keyboard ` and ~
#define KEY_COMMA 0x36 			// Keyboard , and <
#define KEY_DOT 0x37 			// Keyboard . and >
#define KEY_SLASH 0x38 			// Keyboard / and ?
//
//		Function keys F1-F10 are supported.
//
#define KEY_F1 0x3a 			// Return 129..138
//
//		Control keys (Have roughly position control key values)
//
#define KEY_INSERT 0x49 		// Keyboard Insert 					E / 5
#define KEY_HOME 0x4a 			// Keyboard Home 	 				T / 20
#define KEY_PAGEUP 0x4b 		// Keyboard Page Up 				R / 18
#define KEY_DELETE 0x4c 		// Keyboard Delete Forward 			H / 8
#define KEY_END 0x4d 			// Keyboard End 					G / 7
#define KEY_PAGEDOWN 0x4e 		// Keyboard Page Down 				F / 6
#define KEY_RIGHT 0x4f 			// Keyboard Right Arrow 			D / 4
#define KEY_LEFT 0x50 			// Keyboard Left Arrow 				A / 1
#define KEY_DOWN 0x51 			// Keyboard Down Arrow				S / 19
#define KEY_UP 0x52 			// Keyboard Up Arrow 				W / 23

#define KEY_102ND 0x64 			// Keyboard Non-US \ | 	 

#define KBD_MAX_KEYCODE (0x65)	// The biggest scancode we store, +1

