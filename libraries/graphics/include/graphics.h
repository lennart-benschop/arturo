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
	int xOffset,yOffset;  															// Positional offset when drawing.
} GFXPort;

void GFXPortInitialise(GFXPort *vp,int x,int y,int w,int h);
void GFXScrollPort(GFXPort *vp,int xo,int yo);

void GFXPlot(GFXPort *vp,int x,int y,int colour);
void GFXLine(GFXPort *vp,int x0, int y0, int x1, int y1,int colour);
void GFXHLine(GFXPort *vp,int x0, int x1, int y,int colour);
void GFXVLine(GFXPort *vp,int x, int y0, int y1,int colour);
void GFXFrame(GFXPort *vp,int x0,int x1,int y0,int y1,int colour);
void GFXRectangle(GFXPort *vp,int x0,int y0,int x1,int y1,int colour);

