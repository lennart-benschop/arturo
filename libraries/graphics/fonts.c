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

#include "__fontdata.h"

static int _DrawCharacter(GFXPort *vp,int xPos,int yPos,int ch,int font,int colour,int scale);

// ***************************************************************************************
//
//          Draw a single character, return the x adjustment to the next character.
//
// ***************************************************************************************

static int _DrawCharacter(GFXPort *vp,int xPos,int yPos,int ch,int fontid,int colour,int scale) {

    if (fontid < 0 || fontid >= FONT_COUNT) return 0;                                   // Unknown font
    const uint8_t *font = _font_list[fontid];

    if (ch < font[1] || ch > font[2]) return 0;                                     // Unknown character.
    int offset = 8 + (ch - font[1]) * 2;                                            // Address of the offset
    offset = font[offset] + (font[offset+1] << 8);                                  // Convert to the actual offset.
    if (offset == 0xFFFF) return 0;                                                 // Font not defined.
    const uint8_t *chData = font + offset;                                          // Address of the font character record
    const uint8_t *bitmapData = chData+6;                                           // Binary data from here.
    uint8_t bitMask = 0x80;                                                         // Bit to check.

    int height = font[0];
    int width = chData[0];
    GFXASetPort(vp);

    yPos -= (scale * chData[2]) - 1;                                                // Adjust for baseline.
    xPos -= (scale * (chData[1] >> 4));                                             // Kerning
    // TODO: Fill background ?
    yPos += (scale * chData[3]);                                                    // Adjust top gap.

    for (int y = 0;y < chData[4];y++) {                                             // For each line of actual data.
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
    return (width*scale + 1 + (chData[1] & 0x0F));                                  // Return size.
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