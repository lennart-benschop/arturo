// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      fontstructures.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//					Adafruit
//      Date :      3rd January 2025
//      Reviewed :  No
//      Purpose :   Monochrome Font Rendering structures
//
// **************************************************************************************

#pragma once 

typedef struct {
	uint16_t bitmapOffset;  		// Pointer into GFXfont->bitmap
	uint8_t width;          		// Bitmap dimensions in pixels
	uint8_t height;         		// Bitmap dimensions in pixels
	uint8_t xAdvance;       		// Distance to advance cursor (x axis)
	int8_t xOffset;         		// X dist from cursor pos to UL corner
	int8_t yOffset;         		// Y dist from cursor pos to UL corner
} FONTGlyph;

typedef struct {
	uint8_t *bitmap;  				// Glyph bitmaps, concatenated
	FONTGlyph *glyph;   			// Glyph array
	uint16_t first;    				// ASCII extents (first char)
	uint16_t last;     				// ASCII extents (last char)
	uint8_t yAdvance;  				// Newline distance (y axis)
} FONTInfo;

