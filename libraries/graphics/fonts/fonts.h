// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      fonts.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
// 					Limor Fried (for Adafruit)
//      Date :      30th December 2024
//      Reviewed :  No
//      Purpose :   Wrapper for installed fonts from Adafruit
//
// ***************************************************************************************
// ***************************************************************************************

#pragma once

//
//		Originally designed for Arduino which supports this.
//
#define PROGMEM

// ***************************************************************************************
//
//		From https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
//
//		A tool for adding more fonts is there as well.
//
// ***************************************************************************************

//
// 		Font data stored PER GLYPH
//
typedef struct {
  uint16_t bitmapOffset; 			// Pointer into GFXfont->bitmap
  uint8_t width;         			// Bitmap dimensions in pixels
  uint8_t height;        			// Bitmap dimensions in pixels
  uint8_t xAdvance;      			// Distance to advance cursor (x axis)
  int8_t xOffset;        			// X dist from cursor pos to UL corner
  int8_t yOffset;        			// Y dist from cursor pos to UL corner
} GFXglyph;

//
// 		Data stored for FONT AS A WHOLE
//
typedef struct {
  uint8_t *bitmap;  				// Glyph bitmaps, concatenated
  GFXglyph *glyph;  				// Glyph array
  uint16_t first;   				// ASCII extents (first char)
  uint16_t last;    				// ASCII extents (last char)
  uint8_t yAdvance; 				// Newline distance (y axis)
} GFXfont;

//
//		These files all need to have the Adafruit include removed. The simplest way
//		is with a global search and replace
//
#include "FreeSans9pt7b.h"
#include "FreeSerifItalic12pt7b.h"
