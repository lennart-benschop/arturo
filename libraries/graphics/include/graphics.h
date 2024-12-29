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
void GFXAPlot(int x,int y,int colour);
void GFXAHorizLine(int x1,int x2,int y,int colour);
void GFXAVertLine(int x,int y1,int y2,int colour);
void GFXALine(int x0, int y0, int x1, int y1,int colour);

void GFXAUp(void);
void GFXADown(void);
void GFXALeft(void);
void GFXARight(void);
