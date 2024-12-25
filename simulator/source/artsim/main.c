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

// ***************************************************************************************
//
//		Co-opt the USB updating routine, which doesn't happen in the simulator, to 
//		update the simulator, removing events from the queue and repainting the
//		display.
//
// ***************************************************************************************

int USBUpdate(void) {
	tick50HzHasFired = true;
	return SYSPollUpdate();
}

int main(int argc,char *argv[]) {
	DVISetMode(DVI_MODE_640_240_8);
	CONInitialise();
	KBDReceiveEvent(0,0xFF,0);
	FSYSInitialise();
	SYSOpen();
	TESTApplication();
	SYSClose();
	return(0);
}


