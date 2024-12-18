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
#define DVI_MAX_BITPLANES 	(8)  													// This many bitplanes.

struct DVIModeInformation {
	int width,height;  																// Size of screen
	int bitPlaneCount; 																// Number of bitPlanes
	uint8_t *bitPlane[DVI_MAX_BITPLANES]; 											// Up to 8 bitplanes
	int bitPlaneSize;  																// Byte size of each bitplane.
};

void DVIStart(void);
struct dvi_serialiser_cfg *DVIGetHDMIConfig(void);
struct DVIModeInformation *DVIGetModeInformation(void);

#endif