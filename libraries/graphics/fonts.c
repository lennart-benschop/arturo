// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      fonts.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      30th December 2024
//      Reviewed :  No
//      Purpose :   Monochrome Font Rendering
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "include/atomic.h"

// ***************************************************************************************
//
//		Include the font data in Flash memory. This uses the ADAFruit format (see 
//		Adafuit_GFX.cpp lines 1134-1215 (at present)) method Adafruit_GFX::drawChar
//
// ***************************************************************************************

#include "fonts/fonts.h"

// ***************************************************************************************
//
//          Get the Glyph for the given character and font, NULL if not found.
//
// ***************************************************************************************

static GFXglyph *GFXGetGlyph(const GFXfont *font,int c) {
    if (c < font->first || c > font->last) return NULL;
    return &(font->glyph[c-font->first]);
}

// ***************************************************************************************
//
//          Draw a single character, return the x adjustment to the next character.
//
// ***************************************************************************************

static int _DrawCharacter(GFXPort *vp,int xPos,int yPos,int ch,int colour,int scale) {

    GFXASetPort(vp);
    const GFXfont *font = &freesans7pt7b;

    GFXglyph *glyph = GFXGetGlyph(font,ch);                                         // Extract the glyph, exit if not available
    if (glyph == NULL) return 0;
    
    xPos = xPos+glyph->xOffset*scale;                                               // Adjust the drawing point by the offset
    yPos = yPos+glyph->yOffset*scale;

    uint8_t bitCount = 0;                                                           // Track when we need another byte
    int byteOffset = glyph->bitmapOffset;                                           // Offset into byte data
    uint8_t currentByte = 0xFF;                                                     // Current byte being used

    for (int y = 0;y < glyph->height;y++) {                                         // Do vertical first
        for (int x = 0;x < glyph->width;x++) {                                      // Then horizontal
            if ((bitCount & 7) == 0) currentByte = font->bitmap[byteOffset++];      // Do we need another byte
            bitCount++;

            if ((currentByte & 0x80) != 0) {                                        // If bit on.
                if (scale == 1) {
                    GFXAPlot(xPos+x*scale,yPos+y*scale,colour);
                } else {
                    GFXFillRect(vp,xPos+x*scale,yPos+y*scale,xPos+x*scale+scale-1,yPos+y*scale+scale-1,colour);
                }
            }
            currentByte <<= 1;                                                      // Access next byte
        }
    }
    return glyph->xAdvance;
}

void GFXDrawString(GFXPort *vp,int xPos,int yPos,char *s,int colour,int scale) {
    while (*s != '\0' && xPos < vp->width) {
        int w = _DrawCharacter(vp,xPos,yPos,*s++,colour,scale);
        xPos += w;
    }
}