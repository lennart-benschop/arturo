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
static int mode = DVI_MODE_640_240_8;

// *******************************************************************************************************************************
//
//												Return mode data.
//
// *******************************************************************************************************************************

struct DVIModeInformation *DVIGetModeInformation(void) {
	dvi_modeInfo.mode = mode;  														// Record mode
	dvi_modeInfo.bitPlaneCount = 3;
	dvi_modeInfo.bitPlane[0] = redPlane;
	dvi_modeInfo.bitPlane[1] = greenPlane;
	dvi_modeInfo.bitPlane[2] = bluePlane;
	dvi_modeInfo.bitPlaneDepth = 1;

	switch(mode) {
		case DVI_MODE_640_240_8:  													// 640x480x8 information.
			dvi_modeInfo.width = 640;dvi_modeInfo.height = 240;
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
	struct DVIModeInformation *dm = DVIGetModeInformation();
	uint8_t *pr,*pg,*pb,r,g,b;
	SDL_Rect rc;
	for (int y = 0;y < dm->height;y++) {
		rc.w = AS_SCALE * 640/dm->width;
		rc.h = AS_SCALE * 480/dm->height;
		rc.y = y*rc.h;
		pr = dm->bitPlane[0]+y*dm->bytesPerLine;
		pg = dm->bitPlane[1]+y*dm->bytesPerLine;
		pb = dm->bitPlane[2]+y*dm->bytesPerLine;
		if (y == 0) *pr = *pg = *pb = 0x80;
		for (int x = 0;x < dm->width;x+= 8) {
			rc.x = x*rc.w;
			r = *pr++;g = *pg++;b = *pb++;
			for (int bt = 0;bt < 8;bt++) {
				SYSRectangle(&rc,palette[((r & 0x80) >> 7)+((g & 0x80) >> 6)+((b & 0x80) >> 5)]);
				r <<= 1;g <<= 1;b <<= 1;
				rc.x += rc.w;
			}
		}
	}	
	//for (int i = 0;i < 640*2;i++) { printf("%d ",greenPlane[i]); } printf("\n");
}