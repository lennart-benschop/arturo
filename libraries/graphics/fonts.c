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


    // startWrite();
    // for (yy = 0; yy < h; yy++) {
    //   for (xx = 0; xx < w; xx++) {
    //     if (!(bit++ & 7)) {
    //       bits = pgm_read_byte(&bitmap[bo++]);
    //     }
    //     if (bits & 0x80) {
    //       if (size_x == 1 && size_y == 1) {
    //         writePixel(x + xo + xx, y + yo + yy, color);
    //       } else {
    //         writeFillRect(x + (xo16 + xx) * size_x, y + (yo16 + yy) * size_y,
    //                       size_x, size_y, color);
    //       }
    //     }
    //     bits <<= 1;
    //   }
    // }
    // endWrite();