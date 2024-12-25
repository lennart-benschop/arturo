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
	static CTLState state;
	bool butState[4];
	state.dx = state.dy = 0;
	state.a = state.b = state.x = state.y = false;

	Sint16 dx = SDL_JoystickGetAxis(controllers[n],0);
	if (abs(dx) >= 1024) {
		state.dx = (dx < 0) ? -1 : 1;
	}
	Sint16 dy = SDL_JoystickGetAxis(controllers[n],1);
	if (abs(dy) >= 1024) {
		state.dy = (dy < 0) ? -1 : 1;
	}	
	int buttons = SDL_JoystickNumButtons(controllers[n]);
	for (int b = 0;b < 4;b++) {
	 	butState[b] = (b >= buttons) ? false:SDL_JoystickGetButton(controllers[n],b);
	}
	state.a = butState[1];
	state.b = butState[2];
	state.x = butState[0];
	state.y = butState[3];
	return &state;
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
		}
	}
	printf("%d controllers\n",controllerCount);
}
