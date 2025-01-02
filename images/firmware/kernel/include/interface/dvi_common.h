// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		dvi_common.h
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		18th December 2024
//		Reviewed :	No
//		Purpose :	Common values, DVI driver
//
// ***************************************************************************************
// ***************************************************************************************

#pragma once
//
//		Structure for mode information.
//
#define DVI_MODE_640_240_8 		(0)   												// Modes
#define DVI_MODE_320_240_8		(1)
#define DVI_MODE_640_480_2              (2)
#define DVI_MODE_320_240_64             (3)            
#define DVI_MODE_320_256_8              (4)            // Trick mode to get BBC Basic compatible 32 text rows.

//
//		DVI Mode information structure
//
#define DVI_MAX_BITPLANES 	(8)  													// Not very likely, but you never know.

struct DVIModeInformation {
	int mode;  																		// Current Mode.
	int width,height;  																// Size of screen
	int bitPlaneCount; 																// Number of bitPlanes
	int bitPlaneDepth;   															// Bitplane depth in bits.
	int bytesPerLine;  																// Bytes per line of display.
	uint8_t *bitPlane[DVI_MAX_BITPLANES]; 											// Up to 8 bitplanes	
	int bitPlaneSize;  																// Byte size of each bitplane.
};


void DVISetMonoColour(int fg, int bg);
bool DVISetMode(int mode);
struct DVIModeInformation *DVIGetModeInformation(void);

