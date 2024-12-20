// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      keyboard.cpp
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      18th December 2024
//      Reviewed :  No
//      Purpose :   Converts keyboard events to a queue and key state array.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

#define MAX_QUEUE_SIZE (64) 													// Max size of keyboard queue.

//
//		Bit patterns for the key states. These represent the key codes (see kbdcodes.h)
//		at 8 states per byte, so keycode 0 is byte 0 bit 0, keycode 7 is byte 0 bit 7
//		keycode 8 is byte 1 bit 0 etc.
//
static uint8_t keyboardState[KBD_MAX_KEYCODE+1];
static uint8_t keyboardModifiers;
//
//		Queue of ASCII keycode presses.
//
static uint8_t queue[MAX_QUEUE_SIZE+1];
static uint8_t queueSize = 0;

static uint8_t currentASCII = 0,currentKeyCode = 0; 							// Current key pressed.
static uint32_t nextRepeat = 9999;  											// Time of next repeat.

static uint8_t KBDMapToASCII(uint8_t keyCode,uint8_t modifiers);
static uint8_t KBDDefaultASCIIKeys(uint8_t keyCode,uint8_t isShift);
static uint8_t KBDDefaultControlKeys(uint8_t keyCode,uint8_t isShift);

// ***************************************************************************************
//
//					Handle a key event. Note keyCode = 0xFF is reset
//
// ***************************************************************************************

void KBDReceiveEvent(uint8_t isDown,uint8_t keyCode,uint8_t modifiers) {

	if (isDown && keyCode == KEY_ESC) {   										// Pressed ESC
		// TODO: Might want to notify ESC pressed.
	}

	if (keyCode == 0xFF) { 														// Reset request
		queueSize = 0; 															// Empty keyboard queue
		for (unsigned int i = 0;i < sizeof(keyboardState);i++) {   				// No keys down.
			keyboardState[i] = 0; 
		}
		return;
	}

	if (keyCode != 0 && keyCode < KBD_MAX_KEYCODE) { 							// Legitimate keycode.
		if (isDown) {
			keyboardState[keyCode] = 0xFF; 										// Set down flag.
			keyboardModifiers = modifiers;										// Copy modifiers
			uint8_t ascii = KBDMapToASCII(keyCode,modifiers);  					// What key ?
			if (ascii != 0) {
				currentASCII = ascii;  											// Remember code and time.
				currentKeyCode = keyCode;
				KBDInsertQueue(ascii);  										// Push in the queue
				nextRepeat = TMRRead()+KBD_REPEAT_START;
			}
		} else {
			keyboardState[keyCode] = 0x00; 										// Clear flag
			keyboardModifiers = 0x00;											// Clear Modifiers
			if (keyCode == currentKeyCode) currentASCII = 0; 					// Autorepeat off, key released.
		}
	}
}

// ***************************************************************************************
//
//								Handle auto Repeat
//
// ***************************************************************************************

void KBDCheckTimer(void) {
	#if ARTURO_PROCESS_KEYS != 0  												// If not processing keys this is a no-operation.
	if (currentASCII != 0) {  													// Key pressed
		if (TMRRead() >= nextRepeat) {  										// Time up ?
			KBDInsertQueue(currentASCII);  										// Put in queue
			nextRepeat = TMRRead()+KBD_REPEAT_AFTER; 							// Quicker repeat after first time.
		}
	}
	#endif
}

// ***************************************************************************************
//
//								Access the keyboard state
//
// ***************************************************************************************

uint8_t *KBDGetStateArray(void) {
	return keyboardState;
}

uint8_t KBDGetModifiers(void) {
	return keyboardModifiers;
};

// ***************************************************************************************
//
//						Insert ASCII character into keyboard queue
//
// ***************************************************************************************

void KBDInsertQueue(uint8_t ascii) {
	if (queueSize < MAX_QUEUE_SIZE) {   										// Do we have a full queue ?
		queue[queueSize] = ascii;  												// If not insert it.
		queueSize++;
	}
}

// ***************************************************************************************
//
//							  Key available in ASCII queue
//
// ***************************************************************************************

bool KBDIsKeyAvailable(void) {
	return queueSize != 0;
}

// ***************************************************************************************
//
//						Dequeue first key, or 0 if no key available
//
// ***************************************************************************************

uint8_t KBDGetKey(void) {
	if (queueSize == 0) return 0; 												// Queue empty.
	uint8_t key = queue[0];
	for (uint8_t i = 0;i < queueSize;i++) queue[i] = queue[i+1];  				// Dequeue it
	queueSize--;
	return key;
}

// ***************************************************************************************
//
//							Map scancode and modifier to ASCII
//
// ***************************************************************************************

static uint8_t KBDMapToASCII(uint8_t keyCode,uint8_t modifiers) {
	uint8_t ascii = 0;
	uint8_t isShift = (modifiers & KEY_SHIFT) != 0;
	uint8_t isControl = (modifiers & KEY_CONTROL) != 0;

	if (keyCode >= KEY_A && keyCode < KEY_A+26) { 								// Handle alphabet.
		ascii = keyCode - KEY_A + 'A';  										// Make ASCII
		if (!isShift) ascii += 'a'-'A'; 										// Handle shift
		if (isControl) ascii &= 0x1F; 											// Handle control
	}

	if (ascii == 0 && keyCode >= KEY_1 && keyCode < KEY_1+10) { 				// Handle numbers - slightly mangled.
		ascii = (keyCode == KEY_1+9) ? '0' : keyCode - KEY_1 + '1'; 			// because the order on the keyboard isn't 0-9!
		if (isShift) ascii = ")!@#$%^&*("[ascii - '0']; 						// Standard US mapping. of the top row
	}

	if (ascii == 0) {															// This maps all the other ASCII keys.
		ascii = KBDDefaultASCIIKeys(keyCode,modifiers); 						
	}
	if (ascii == 0) {															// This maps all the control keys
		ascii = KBDDefaultControlKeys(keyCode,modifiers); 						
	}

	return LOCLocaleMapping(ascii,keyCode,modifiers); 							// Special mapping for locales.
}

// ***************************************************************************************
//
//						Work out ASCII keys (including space)
//
// ***************************************************************************************

#define KEY(code,normal,shifted) code,normal,shifted

static const uint8_t defaultShift[] = {
	KEY(KEY_MINUS,'-','_'),			KEY(KEY_EQUAL,'=','+'), 		KEY(KEY_LEFTBRACE,'[','{'), 
	KEY(KEY_RIGHTBRACE,']','}'), 	KEY(KEY_BACKSLASH,'\\','|'), 	KEY(KEY_HASHTILDE,'#','~'),
	KEY(KEY_SEMICOLON,';',':'), 	KEY(KEY_APOSTROPHE,'\'','"'), 	KEY(KEY_GRAVE,'`','~'),
	KEY(KEY_COMMA,',','<'), 		KEY(KEY_DOT,'.','>'), 			KEY(KEY_SLASH,'/','?'),
	KEY(KEY_SPACE,' ',' '),			KEY(KEY_102ND,'\\','|'),
	0	
};

static uint8_t KBDDefaultASCIIKeys(uint8_t keyCode,uint8_t isShift) {
	uint8_t ascii = 0;
	uint8_t index = 0;
	while (defaultShift[index] != 0 && defaultShift[index] != keyCode) 			// Look up in table of standard mappings (US)
		index += 3;
	if (defaultShift[index] == keyCode) {  										// found a match.
		ascii = isShift ? defaultShift[index+2] : defaultShift[index+1]; 	
	}
	return ascii;
}

// ***************************************************************************************
//
//						Work out standard controls (include CHR(127))
//
// ***************************************************************************************

static const uint8_t defaultControlKeys[] = {
	KEY_LEFT,CC_LEFT,KEY_RIGHT,CC_RIGHT,KEY_INSERT,CC_INSERT,
	KEY_PAGEDOWN,CC_PAGEDOWN,KEY_END,CC_END,KEY_DELETE,CC_DELETE,
	KEY_TAB,CC_TAB,KEY_ENTER,CC_ENTER,KEY_PAGEUP,CC_PAGEUP,KEY_DOWN,CC_DOWN,
	KEY_HOME,CC_HOME,KEY_UP,CC_UP,KEY_ESC,CC_ESC, 
	KEY_BACKSPACE, CC_BACKSPACE, 0
};	

static uint8_t KBDDefaultControlKeys(uint8_t keyCode,uint8_t isShift) {
	uint8_t index = 0;
	while (defaultControlKeys[index] != 0) {
		if (defaultControlKeys[index] == keyCode) {
			return defaultControlKeys[index+1];
		}
		index += 2;
	} 
	return 0;
}	
