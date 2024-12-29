// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      drawing.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      29th December 2024
//      Reviewed :  No
//      Purpose :   Drawing functions
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "include/atomic.h"

// ***************************************************************************************
//
//								Wrappers for atomic functions
//
// ***************************************************************************************

void GFXPlot(GFXPort *vp,int x,int y,int colour) {
	GFXASetPort(vp);
	GFXAPlot(x,y,colour);
}

void GFXLine(GFXPort *vp,int x0, int y0, int x1, int y1,int colour) {
	GFXASetPort(vp);
	GFXALine(x0,y0,x1,y1,colour);	
}

void GFXHLine(GFXPort *vp,int x0, int x1, int y,int colour) {
	GFXASetPort(vp);
	GFXAHorizLine(x0,x1,y,colour);	
}

void GFXVLine(GFXPort *vp,int x, int y0, int y1,int colour) {
	GFXASetPort(vp);
	GFXAHorizLine(x,y0,y1,colour);	
}

