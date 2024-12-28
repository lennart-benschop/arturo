// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      graphics.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      28th December 2024
//      Reviewed :  No
//      Purpose :   Graphics core header.
//
// ***************************************************************************************
// ***************************************************************************************

#pragma once

#define XXX "Hello"

//
//		A drawing area on the display.
//
typedef struct _GFXPort {
	int x,y,width,height;  															// Viewport position, pixels. 
} GFXPort;

void GFXPortInitialise(GFXPort *vp,int x,int y,int w,int h);


//
//		TODO: Move atomic functions out of scope.
//
void GFXASetPort(GFXPort *vp);
void GFXAMove(int x,int y);
void GFXAPlot(int colour);
