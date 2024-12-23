// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      render.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      23rd December 2024
//      Reviewed :  No
//      Purpose :   Display Rendering.
//
// ***************************************************************************************
// ***************************************************************************************

#include <artsim.h>

static int palette[8] = { 0x000,0xF00,0x0F0,0xFF0,0x00F,0xF0F,0x0FF,0xFFF };

static uint8_t redPlane[640*480/8];
static uint8_t greenPlane[640*480/8];
static uint8_t bluePlane[640*480/8];

static struct DVIModeInformation dvi_modeInfo;
static int mode = DVI_MODE_640_480_8;

// *******************************************************************************************************************************
//
//												Return mode data.
//
// *******************************************************************************************************************************

struct DVIModeInformation *DVIGetModeInformation(void) {
	dvi_modeInfo.mode = mode;  														// Record mode
	dvi_modeInfo.userMemory = NULL;   												// Common defaults.
	dvi_modeInfo.userMemorySize = 0;		
	dvi_modeInfo.bitPlaneCount = 3;
	dvi_modeInfo.bitPlane[0] = redPlane;
	dvi_modeInfo.bitPlane[1] = greenPlane;
	dvi_modeInfo.bitPlane[2] = bluePlane;
	dvi_modeInfo.bitPlaneDepth = 1;

	switch(mode) {
		case DVI_MODE_640_480_8:  													// 640x480x8 information.
			dvi_modeInfo.width = 640;dvi_modeInfo.height = 480;
			dvi_modeInfo.bitPlaneSize = sizeof(redPlane);
			break;
		case DVI_MODE_320_240_8:  													// 320x240x8 information.
			dvi_modeInfo.width = 320;dvi_modeInfo.height = 240;
			dvi_modeInfo.bitPlaneSize = sizeof(redPlane);
			break;
		default:
			dvi_modeInfo.mode = -1;  												// Failed.
	}
	dvi_modeInfo.bytesPerLine = dvi_modeInfo.width / 8;  							// Calculate bytes per line.	return &modeInfo;
	return &dvi_modeInfo;
}

// *******************************************************************************************************************************
//
//												Get elapsed time in 100Hz ticks.
//
// *******************************************************************************************************************************

void RNDRender(SDL_Surface *surface) {	
	static int n = 0xF70;
	static int r,g,b;
	SDL_Rect rc;
	for (int y = 0;y < 480;y++) {
		rc.w = rc.h = AS_SCALE;rc.y = y*AS_SCALE;
		for (int x = 0;x < 640;x+= 8) {
			rc.x = x*AS_SCALE;
			r = x;g = y;b = x+y;
			for (int bt = 0;bt < 8;bt++) {
				SYSRectangle(&rc,palette[((r & 0x80) >> 7)+((g & 0x80) >> 6)+((b & 0x80) >> 5)]);
				rc.x += AS_SCALE;
			}
		}
	}
	//for (int i = 0;i < 640*2;i++) { printf("%d ",greenPlane[i]); } printf("\n");
}