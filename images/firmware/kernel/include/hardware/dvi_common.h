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

#ifndef _DVI_COMMON_H
#define _DVI_COMMON_H
//
//		PicoDVI Configuration
//
#define VREG_VSEL VREG_VOLTAGE_1_20
#define DVI_TIMING dvi_timing_640x480p_60hz
//
//		Structure for mode information.
//
#define DVI_MODE_640_480_8 		(0)   												// Modes.
#define DVI_MODE_320_240_256	(1)

//
//		DVI Mode information structure
//
#define DVI_MAX_BITPLANES 	(8)  													// Not very likely, but you never know.

struct DVIModeInformation {
	int mode;  																		// Current Mode.
	int width,height;  																// Size of screen
	int bitPlaneCount; 																// Number of bitPlanes
	int bitPlaneDepth;   															// Bitplane depth in bits.
	uint8_t *bitPlane[DVI_MAX_BITPLANES]; 											// Up to 8 bitplanes
	int bitPlaneSize;  																// Byte size of each bitplane.
	void *userMemory;  																// Any available RAM, NULL if none.
	int userMemorySize; 															// Byts of user memory.
};

void DVIStart(void);
void DVISetMode(int mode);
struct dvi_serialiser_cfg *DVIGetHDMIConfig(void);
struct DVIModeInformation *DVIGetModeInformation(void);

#endif