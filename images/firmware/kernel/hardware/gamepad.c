// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      gamepad.c
//      Authors :   Paul Robson (paul@robsons.org.uk), based on work by Sascha Schneider
//      Date :      20th December 2024
//      Reviewed :  No
//      Purpose :   Gamepad controller class
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

#define MAX_CONTROLLERS 	(4) 													// In practice, only one can plug in !

static int controllerCount;  														// Number of controllers
static CTLState controllers[MAX_CONTROLLERS];  										// Controller states.

// ***************************************************************************************
//
//								Initialise the controller system
//
// ***************************************************************************************

void CTLInitialise(void) {
	controllerCount = 0;
}

// ***************************************************************************************
//
//							Get the number of controllers plugged in.
//
// ***************************************************************************************

int  CTLControllerCount(void) {
	return controllerCount;
}

// ***************************************************************************************
//
//					Add a specific controller device to the device lists
//
// ***************************************************************************************

void CTLAddController(uint8_t dev_addr,uint8_t instance,uint16_t vid,uint16_t pid) {
	if (controllerCount < MAX_CONTROLLERS) {  										// If not reached the end.
		CONWriteString("%04x:%04x %02x %02x\r",vid,pid,dev_addr,instance);
		CTLState *cs = &controllers[controllerCount]; 								// Pointer to new controller
		cs->_hardwareID = CTL_HARDWARE_ID(dev_addr,instance);  						// Store the hardware ID, and the type ID.
		cs->_hardwareTypeID = CTL_DEVICE_TYPE_ID(vid,pid);
		cs->dx = cs->dy = 0;cs->a = cs->b = cs->x = cs->y = false;   				// Reset the rest

		// Check if it is a legitimate controller, if so bump controllerCount
	} 
}

// ***************************************************************************************
//
//					Update a controller, using the report and length.
//
// ***************************************************************************************

void CTLUpdateController(uint8_t dev_addr,uint8_t instance,uint8_t const *report,uint8_t len) {
	CONWriteString("%d : ",len);
	for (int i = 0;i < len;i++) CONWriteString("%d:%02x ",i,report[i]);
	CONWriteString("\r");
}
