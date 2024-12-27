// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      main.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      23rd December 2024
//      Reviewed :  No
//      Purpose :   Main program for Simulator.
//
// ***************************************************************************************
// ***************************************************************************************

#include "artsim.h"

bool tick50HzHasFired = true;

#define FRAME_RATE 	(50)

static int nextUpdateTime = 0;

// ***************************************************************************************
//
//		Co-opt the USB updating routine, which doesn't happen in the simulator, to 
//		update the simulator, removing events from the queue and repainting the
//		display. This is because there is no tick interrupt.
//
// ***************************************************************************************

int USBUpdate(void) {
	tick50HzHasFired = true;  														// Rigged so we fire every time round the main loop
	if (TMRRead() >= nextUpdateTime) {   											// So do this to limit the repaint rate to 50Hz.
		nextUpdateTime = TMRRead()+100/FRAME_RATE;
		return SYSPollUpdate();  													// So we don't update *all* the time
	}
	return -1;
}

// ***************************************************************************************
//
//											Main program
//
// ***************************************************************************************

int main(int argc,char *argv[]) {
	DVISetMode(DVI_MODE_640_240_8);
	CONInitialise();
	KBDReceiveEvent(0,0xFF,0);
	FIOInitialise();
	SYSOpen(true);  																// Shuts up the audio :)
	LOGODrawTitle();
	LOGOTextHeader();
	TESTApplication();
	SYSClose();
	return(0);
}
