// ***************************************************************************************
// ***************************************************************************************
//
//      Name :     	gamepad.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      20th December 2024
//      Reviewed :  No
//      Purpose :   Gamepad header
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _GAMEPAD_H
#define _GAMEPAD_H

#define CTL_DEVICE_TYPE_ID(vid,pid) 			(((vid) << 16)|(pid))  				// 32 bit device type.
#define CTL_HARDWARE_ID(dev_addr,instance)		(((dev_addr << 8)|(instance))) 		// 16 bit composite identifier

// ***************************************************************************************
//
//		Structure containing the controller state. Members with _ in front of 
//		their identifier should be considered private.
//		
//		We simplify controllers so they have one D-Pad and 4 fire buttons.
//
// ***************************************************************************************

typedef struct _controller_status {
	uint16_t	_hardwareID;   														// 16 bit hardware ID built from dev_addr & instance	
	uint32_t 	_hardwareTypeID;  													// Combined VID & PID of the device.

	int  		dx,dy;   															// Direction of the main controller
	bool 		a,b,x,y;  															// Buttons. These should map with the SNES controller
																					// So a PSX would be Circle (A) Cross (X) Triangle (X) Square (Y)
} CTLState;

#define CTLM_REGISTER		(0)
#define CTLM_UPDATE 		(1)

struct _CTL_MessageData {
	const uint8_t *report;   														// USB report data
	uint8_t len;  																	// Length of report data.
};

void CTLInitialise(void);
void CTLAddController(uint8_t dev_addr,uint8_t instance,uint16_t vid,uint16_t pid);
void CTLUpdateController(uint8_t dev_addr,uint8_t instance,uint8_t const *report,uint8_t len);
int  CTLSendMessage(int command,uint16_t hwid,struct _CTL_MessageData *msg);
int  CTLDispatchMessage(int command,CTLState *cs,struct _CTL_MessageData *msg);
int  CTLControllerCount(void);
void CTLAnnounceDevice(CTLState *cs,const char *name);
CTLState *CTLReadController(int n);

#endif
