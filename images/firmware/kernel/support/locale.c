// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      locale.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      19th December 2024
//      Reviewed :  No
//      Purpose :   Locale Handling
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

static bool _LOCMatchModifiers(uint8_t modState,uint8_t modRequired);

// ***************************************************************************************
//
//								Locale Data
//
// ***************************************************************************************

static const uint8_t _KBDLocaleData[] = {
	#include "data/__locale.h"
};

static const uint8_t *_KBDLocaleCurrent = NULL;

// ***************************************************************************************
//
//									Set Locale
//
// ***************************************************************************************

void LOCSetLocale(char *loc) {
	bool succeed = false;
	char c1 = tolower(loc[0]);
	char c2 = tolower(loc[1]);	
	const uint8_t *search = _KBDLocaleData;
	_KBDLocaleCurrent = NULL;
	while (search[0] != 0) {  													// Look through the locale table
		if (c1 == search[1] && c2 == search[2]) {  								// Found the locale, use it
			_KBDLocaleCurrent = search+3;	
			succeed = true;
		}  	
		search += search[0];													// Follow the list.
	}
	if (!succeed) CONWriteString("Setting the locale to '%s' failed.\r",loc);
}

// ***************************************************************************************
//
//								Process locale fixes
//
// ***************************************************************************************

uint8_t LOCLocaleMapping(uint8_t asciiCode,uint8_t keyCode,uint8_t modifiers) {

	if (_KBDLocaleCurrent != NULL) {  											// Is there a locale conversion ?	
		const uint8_t *keyCheck = _KBDLocaleCurrent; 							// Scan the locale list
		while (*keyCheck != 0xFF) {
			if (_LOCMatchModifiers(modifiers,keyCheck[0]) &&   					// If the modifiers match
				keyCheck[1] == keyCode) {   									// And the keycode match
				 	asciiCode = keyCheck[2];  									// Use this one.
			}
			keyCheck += 3;
		}
	}
	return asciiCode;
}

// ***************************************************************************************
//
//								Check modifiers match
//
// ***************************************************************************************

static bool _LOCMatchModifiers(uint8_t modState,uint8_t modRequired) {
	if (modRequired == KEY_NOSHIFT) {  											// Not-shifted is a special case.
		return (modState == 0);
	} else {  																	// All other modifiers
		return (modState & modRequired) != 0; 
	}
}

// ***************************************************************************************
//
//		Date 		Revision
//		==== 		========
//
// ***************************************************************************************
