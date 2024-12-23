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

// ***************************************************************************************
//
//		Co-opt the USB updating routine, which doesn't happen in the simulator, to 
//		update the simulator, removing events from the queue and repainting the
//		display.
//
// ***************************************************************************************

int USBUpdate(void) {
	return SYSPollUpdate();
}

int main(int argc,char *argv[]) {
	SYSOpen();
	TESTApplication();
	SYSClose();
	return(0);
}

