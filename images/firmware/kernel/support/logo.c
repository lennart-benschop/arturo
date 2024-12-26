// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      logo.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      26th December 2024
//      Reviewed :  No
//      Purpose :   Draw Logo
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

#include "support/__logo_data.h"

void LOGODrawTitle(void) {
    LOGODraw(DVIGetModeInformation()->width-LOGO_WIDTH_BYTES*8-4,4,LOGO_WIDTH_BYTES,LOGO_HEIGHT,logoData,1,0);
}

void LOGODraw(int x,int y,int w,int h,const uint8_t *pixelData,int fgr,int bgr) {
    for (int yc = 0;yc < h;yc++) {
        for (int xb = 0;xb < w;xb++) {
            for (int bit = 0;bit < 8;bit++) {
                CONDrawPixel(x+xb*8+bit,y+yc,(*pixelData & (0x80 >> bit)) ? fgr:bgr);
            }
            pixelData++;
        }
    }
}
