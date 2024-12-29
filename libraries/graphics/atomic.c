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

static void _GFXAValidate(void);

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
static int width,height;  															// Size

#define CONV_X(x) 		(x)
#define CONV_Y(y) 		(y)
#define OFFWINDOWH(x) 	((x) < 0 || (x) >= width)
#define OFFWINDOWV(y) 	((y) < 0 || (y) >= height)
#define OFFWINDOW(x,y) 	(OFFWINDOWH(x) || OFFWINDOWV(y))

// ***************************************************************************************
//
//								Set the current viewport
//
// ***************************************************************************************

void GFXASetPort(GFXPort *vp) {
	_currentPort = vp;  															// Save viewport
	_dmi = DVIGetModeInformation();  												// Record current information
	width = _currentPort->width;height = _currentPort->height;  					// Temporary variables.
	dataValid = false;  															// Nothing is set up.
}

// ***************************************************************************************
//
//							Plot a pixel in the current port
//
// ***************************************************************************************

void GFXAPlot(int x,int y,int colour) {
	xPixel = CONV_X(x);yPixel = CONV_Y(y);  										// Update the pixel positions.
	_GFXAValidate();  																// Validate the position.
	if (dataValid) _GFXDrawBitmap(colour);  										// Draw pixel if valid.
}

// ***************************************************************************************
//
//							Try to validate pixel position
//
// ***************************************************************************************

static void _GFXAValidate(void) {
	dataValid = false;
	if (OFFWINDOW(xPixel,yPixel)) return; 											// No, we can't do anything.
	int xc = xPixel+_currentPort->x;  												// Physical position on screen
	int yc = yPixel+_currentPort->y;
	int offset = (xc >> 3) + (yc * _dmi->bytesPerLine); 							// Work out the offset on the bitmap planes.
	pl0 = _dmi->bitPlane[0]+offset;  												// Set up bitmap plane pointers.
	pl1 = _dmi->bitPlane[1]+offset;  
	pl2 = _dmi->bitPlane[2]+offset;  
	bitMask = (0x80 >> (xc & 7)); 													// Work out the bitmask for the current pixel.
	dataValid = true;  																// We have valid data
}

// ***************************************************************************************
//
//									Horizontal line
//
// ***************************************************************************************

void GFXAHorizLine(int x1,int x2,int y,int colour) {
	x1 = CONV_X(x1);x2 = CONV_X(x2);y = CONV_Y(y);  								// Convert to physical pixels in window.
	if (OFFWINDOWV(y)) return;  													// Vertically out of range => no line.
	if (x1 >= x2) { int n = x1;x1 = x2;x2 = n; }  									// Sort the x coordinates into order.
	if (x2 < 0 || x1 >= width) return;   											// On screen area (e.g. lower off right, higher off left)
	x1 = max(x1,0);x2 = min(x2,width-1);  											// Trim horizontal line to port.
	xPixel = x1;yPixel = y;dataValid = false;  										// First pixel.
	_GFXAValidate();  
	int pixelCount = x2-x1+1;  														// Pixels to draw 
	while (pixelCount-- > 0) {
		_GFXDrawBitmap(colour);
		GFXARight();
	}
}


// ***************************************************************************************
//
//								Move current position
//
// ***************************************************************************************

void GFXARight(void) {
	xPixel++;  																		// Pixel right
	bitMask >>= 1;  																// Shift bitmap right
	if (bitMask == 0) {  															// Off the right side.
		bitMask = 0x80;  															// Reset bitmap
		pl0++;pl1++;pl2++;  														// Bump plane pointers		
	}
	dataValid = (xPixel < width);  													// Still in window
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
