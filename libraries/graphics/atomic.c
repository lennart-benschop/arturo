// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      atomic.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      28th December 2024
//      Reviewed :  No
//      Purpose :   Lowest level graphics routines
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

static inline void _GFXDrawBitmap1(int colour);
static inline void _GFXDrawBitmap3(int colour);
static inline void _GFXDrawBitmap(int colour);

// ***************************************************************************************
//
//		These functions are atomic. They assume once you've selected the viewport 
//		the mode, port will not change for the duration of any function that uses them
//		e.g. to draw rectangles.
//
// ***************************************************************************************

static GFXPort *_currentPort = NULL;  												// Current viewport
static struct DVIModeInformation *_dmi = NULL;  									// Current mode information.
static int xPixel,yPixel;  															// Pixel position in current window
static bool dataValid;  															// True if data is valid.
static uint8_t bitMask;  															// Bitmask (when data is valid)
static uint8_t *pl0,*pl1,*pl2;  													// Bitplane pointers.
static int x1,y1,x2,y2;  															// Port precalculated

#define CONV_X(x) 		(x)
#define CONV_Y(y) 		(y)
#define OFFWINDOW(x,y) 	((x) < x1 || (y) < y1 || (x) > x2 || (y) > y2)

// ***************************************************************************************
//
//								Set the current viewport
//
// ***************************************************************************************

void GFXASetPort(GFXPort *vp) {
	_currentPort = vp;  															// Save viewport
	_dmi = DVIGetModeInformation();  												// Record current information
	dataValid = false;  															// Nothing is set up.
	x1 = vp->x;y1 = vp->y;x2 = vp->x+vp->width-1;y2 = vp->y+vp->height-1;  			// Calculate bounding box
}

// ***************************************************************************************
//
//							Move the drawing position absolute
//
// ***************************************************************************************

void GFXAMove(int x,int y) {
	xPixel = CONV_X(x);yPixel = CONV_Y(y);  										// Update the pixel positions.
	dataValid = false;  															// It may be valid, we'll check if we do something.
}

void GFXAPlot(int colour) {
	if (!dataValid) {  																// Data valid ?
		if (OFFWINDOW(xPixel,yPixel)) return;  										// No, we can't do anything.
		int offset = (xPixel >> 3) + (yPixel * _dmi->bytesPerLine); 				// Work out the offset.
		pl0 = _dmi->bitPlane[0]+offset;  											// Set up bitmap plane pointers.
		pl1 = _dmi->bitPlane[1]+offset;  
		pl2 = _dmi->bitPlane[2]+offset;  
		bitMask = (0x80 >> (xPixel & 7)); 											// Work out the bit
		dataValid = true;  															// We have valid data
	}
	if (dataValid) _GFXDrawBitmap(colour);

}

// ***************************************************************************************
//
//								Draw the current pixel
//
// ***************************************************************************************

static inline void _GFXDrawBitmap(int colour) {
	if (!dataValid) return;              											// Not valid drawing.
	if (_dmi->bitPlaneCount == 1) {  												// Draw the bitmap.
		_GFXDrawBitmap1(colour);
	} else { 
		_GFXDrawBitmap3(colour); 
	}
}

// ***************************************************************************************
//
//							Draw the current pixel (3 planes)
//
// ***************************************************************************************

static inline void _GFXDrawBitmap3(int colour) {
	*pl0 = ((*pl0) & (~bitMask)) | ((colour & 1) ? bitMask:0);
	*pl1 = ((*pl1) & (~bitMask)) | ((colour & 2) ? bitMask:0);
	*pl2 = ((*pl2) & (~bitMask)) | ((colour & 4) ? bitMask:0);
}

// ***************************************************************************************
//
//							Draw the current pixel (1 plane)
//
// ***************************************************************************************

static inline void _GFXDrawBitmap1(int colour) {
	*pl0 = ((*pl0) & (~bitMask)) | ((colour & 1) ? bitMask:0);
}
