// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      gamepad.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      25th December 2024
//      Reviewed :  No
//      Purpose :   Controller code.
//
// ***************************************************************************************
// ***************************************************************************************

#include <common.h>
#include <artsim.h>

#define MAX_CONTROLLERS (4)

static int controllerCount = 0;
static SDL_Joystick *controllers[MAX_CONTROLLERS];

// *******************************************************************************************************************************
//
//												    Get controller count
//
// *******************************************************************************************************************************

int CTLControllerCount(void) {
	return controllerCount;
}

// *******************************************************************************************************************************
//
//										Read controller, format compatible with Firmware
//
// *******************************************************************************************************************************

CTLState *CTLReadController(int n) {
	Sint16 dx = SDL_JoystickGetAxis(controllers[n],0);
	if (abs(dx) >= 1024) {
//		bitPattern |= (dx < 0) ? 0x01:0x02;
	}
	Sint16 dy = SDL_JoystickGetAxis(controllers[n],1);
	if (abs(dy) >= 1024) {
//		bitPattern |= (dy < 0) ? 0x04:0x08;
	}
	// int buttons = SDL_JoystickNumButtons(controllers[n]);
	// buttons = (buttons >= 4) ? 4 : buttons;
	// for (int b = 0;b < buttons;b++) {
	// 	if (SDL_JoystickGetButton(controllers[n],b)) {
	// 		bitPattern |= (0x10 << b);
	// 	}
	// }
	return NULL;
}

// *******************************************************************************************************************************
//
//												    Search for controllers
//
// *******************************************************************************************************************************

void CTLFindControllers(void) {
 	controllerCount = 0;  															// Discover controllers. 
	for (int i = 0; i < SDL_NumJoysticks(); i++) {
		if (controllerCount < MAX_CONTROLLERS) {
    		controllers[controllerCount] = SDL_JoystickOpen(i);
    		if (controllers[controllerCount] == NULL) {
    			exit(printf("Failed to open controller %d\n",i));
    		}
    		controllerCount++;
    		printf("Found controller\n");
		}
	}
	printf("%d controllers\n",controllerCount);
}
