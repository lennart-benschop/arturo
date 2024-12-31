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

#include "__fonts.h"

// ***************************************************************************************
//
//          Draw a single character, return the x adjustment to the next character.
//
// ***************************************************************************************

static int _DrawCharacter(GFXPort *vp,int xPos,int yPos,int ch,int colour,int scale) {

    GFXASetPort(vp);
    return 0;
}

void GFXDrawString(GFXPort *vp,int xPos,int yPos,char *s,int colour,int scale) {
    while (*s != '\0' && xPos < vp->width) {
        int w = _DrawCharacter(vp,xPos,yPos,*s++,colour,scale);
        xPos += w;
    }
}