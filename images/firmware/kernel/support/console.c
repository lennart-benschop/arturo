// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		console.c
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		18th December 2024
//		Reviewed :	No
//		Purpose :	Console output code, for debugging primarily.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

#include "support/font_8x8.h"

#define FONT_CHAR_WIDTH 8
#define FONT_CHAR_HEIGHT 8
#define FONT_N_CHARS 95
#define FONT_FIRST_ASCII 32

// ***************************************************************************************
//
//						Set one pixel at x,y in the given colour.
//
// ***************************************************************************************

static inline void putpixel(uint x, uint y, uint rgb) {
	struct DVIModeInformation *dmi = DVIGetModeInformation();  						// Identify mode data.
	if (dmi == NULL) return;
	uint8_t mask = 0x80 >> (x % 8);  												// Mask from lower 8 bits.

	for (uint component = 0; component < dmi->bitPlaneCount; ++component) {  		// Do each bitplane
		uint8_t *idx = (x / 8) + y * dmi->bytesPerLine + dmi->bitPlane[component]; 	// Work out byte.

		if (rgb & (1u << component))  												// Set or clear the bit in the plane.
			*idx = *idx | mask;
		else
			*idx = *idx & (~mask);
	}
}

// ***************************************************************************************
//
//								Console setup - very simple here.
//
// ***************************************************************************************

static uint x0 = 0;  																// Posiiton in pixels
static uint y0 = 0;
static uint fgcol = CON_COL_WHITE;  												// Text colour
static uint bgcol = CON_COL_BLACK;

// ***************************************************************************************
//
//									Console Initialise
//
// ***************************************************************************************

void CONInitialise(void) {
	CONWrite(12);																	// CHR(12) is clear screen.
}

// ***************************************************************************************
//
//							Write one character or control code.
//
// ***************************************************************************************

void CONWrite(char c) {
	struct DVIModeInformation *dmi = DVIGetModeInformation();  						// Identify mode data.
	if (dmi == NULL) return;

	switch(c) {
		case 12:  																	// Clear Screen.
			for (uint x = 0; x < dmi->width; ++x)
				for (uint y = 0; y < dmi->height; ++y)
					putpixel(x, y, CON_COL_BLACK);			
			fgcol = CON_COL_WHITE;													// Reset colours.
			bgcol = CON_COL_BLACK;
			x0 = y0 = 0;  															// Home cursor
			for (int i = 0;i < 240;i++) putpixel(i,i,i & 7);
			break;
		case 10:
		case 13:  																	// New line
			x0 = 0;y0 = y0 + 8;
			if (y0 == dmi->height) {  												// Scrolling.
				y0 = y0 - 8;  										
				for (int i = 0;i < dmi->bitPlaneCount;i++) {
					uint8_t *s = dmi->bitPlane[i];
					memcpy(s,s+8*dmi->bytesPerLine,dmi->bytesPerLine*(dmi->height-8));
					memset(s+dmi->bytesPerLine*(dmi->height-8),0,8*dmi->bytesPerLine);
				}
			}
			break;

		default:
			fgcol = c % 7 + 1;  													// A hack to show the colours :)
			if (c >= FONT_FIRST_ASCII && c < FONT_FIRST_ASCII+FONT_N_CHARS) {  		// ASCII character set.
				for (int y = y0; y < y0 + 8; ++y) {
					uint8_t font_bits = font_8x8[(c - FONT_FIRST_ASCII) * FONT_CHAR_HEIGHT+y-y0];
					for (int i = 0; i < 8; ++i) {
						putpixel(x0 + i, y, font_bits & (1 << i) ? fgcol : bgcol);
						}						
					}
				x0 = x0 + 8;
				if (x0 == dmi->width) CONWrite(13);
			}
			break;
	}
}

// ***************************************************************************************
//
//								Write a string in varargs format
//
// ***************************************************************************************

void CONWriteString(const char *fmt, ...) {
	char buf[128];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buf, 128, fmt, args);
	char *p = buf;
	while (*p != '\0') CONWrite(*p++);
	va_end(args);
}
