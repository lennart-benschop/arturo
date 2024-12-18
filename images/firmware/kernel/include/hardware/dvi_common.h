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

#define FRAME_WIDTH 640
#define FRAME_HEIGHT 480
#define VREG_VSEL VREG_VOLTAGE_1_20
#define DVI_TIMING dvi_timing_640x480p_60hz

#define PLANE_SIZE_BYTES (FRAME_WIDTH * FRAME_HEIGHT / 8)
extern uint8_t framebuf[3 * PLANE_SIZE_BYTES];

void DVIStart(void);

#endif