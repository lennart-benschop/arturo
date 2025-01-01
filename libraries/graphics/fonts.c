// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      fonts.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Andrew S. Owen (Font Design)
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
//		                    Include the font data in Flash memory. 
//
// ***************************************************************************************

#include "fontdata.h"

// ***************************************************************************************
//
//          Draw a single character, return the x adjustment to the next character.
//
// ***************************************************************************************

static int _DrawCharacter(GFXPort *vp,int xPos,int yPos,int ch,int fontid,int colour,int scale) {

    if (fontid < 0 || fontid >= FONT_COUNT) return 0;                               // Unknown font
    const uint8_t *font = fontTable[fontid];
    if (ch < font[0] || ch > font[1]) return 0;                                     // Unknown character.
    int offset = 64 + (ch - font[0]) * 2;                                           // Address of the offset
    offset = font[offset] + (font[offset+1] << 8);                                  // Convert to the actual offset.
    if (offset == 0xFFFF) return 0;                                                 // Font not defined.
    const uint8_t *chData = font + offset;                                          // Address of the font character record
    const uint8_t *bitmapData = chData+3;                                           // Binary data from here.
    uint8_t bitMask = 0x80;                                                         // Bit to check.

    int height = font[2];                                                           // Total font height
    int width = chData[0];                                                          // Width of the glyph
    GFXASetPort(vp);

    yPos = yPos - scale * font[3];                                                  // Go to the top of the character from the baseline
    // TODO: Fill background ?
    yPos = yPos + scale * chData[1];                                                // Go to the top of the data.

    for (int y = 0;y < chData[2];y++) {                                             // For each line of actual data.
        for (int x = 0;x < width;x++) {                                             // Each horizontal pixel ?
            if (((*bitmapData) & bitMask) != 0) {                                   // Plot bitmap.
                if (scale == 1) {                                                   // Pixel or Rectangle.
                    GFXAPlot(xPos+x*scale,yPos,colour);
                } else {
                    GFXFillRect(vp,xPos+x*scale,yPos,xPos+x*scale+scale-1,yPos+scale-1,colour);
                }
            }            
            bitMask >>= 1;                                                          // Next bit
            if (bitMask == 0) {
                bitMask = 0x80;bitmapData++;
            }
        }
        yPos += scale;                                                              // Next line down.
    }
    offset = scale * chData[0];                                                     // Actual data offset, no spacing.
    return offset + max(1,height/10);
}

// ***************************************************************************************
//
//                                      Draw font
//
// ***************************************************************************************

void GFXDrawString(GFXPort *vp,int xPos,int yPos,char *s,int font,int colour,int scale) {
    while (*s != '\0' && xPos < vp->width) {
        int w = _DrawCharacter(vp,xPos,yPos,*s++,font,colour,scale);
        xPos += w;
    }
}

