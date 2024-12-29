// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      port.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      28th December 2024
//      Reviewed :  No
//      Purpose :   Graphic Port functions
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

// ***************************************************************************************
//
//									Initialise a Graphics Port
//
// ***************************************************************************************

void GFXPortInitialise(GFXPort *vp,int x,int y,int w,int h) {
	struct DVIModeInformation *dmi = DVIGetModeInformation();
	vp->x = min(dmi->width-1,max(0,x));  											// Assign position and size forcing into current mode.
	vp->y = min(dmi->height-1,max(0,y));
	vp->width = min(dmi->width-vp->x,w);
	vp->height = min(dmi->height-vp->y,h);
	vp->xOffset = vp->yOffset = 0;
}

// ***************************************************************************************
//
//							Scroll port offset to new position
//
// ***************************************************************************************

void GFXScrollPort(GFXPort *vp,int xo,int yo) {
	vp->xOffset = xo;vp->yOffset = yo;
}


