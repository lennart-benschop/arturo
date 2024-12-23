// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		dvi_common.h
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		18th December 2024
//		Reviewed :	No
//		Purpose :	Common values, DVI driver (internal)
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _DVI_COMMONI_H
#define _DVI_COMMONI_H
//
//		PicoDVI Configuration
//
#define VREG_VSEL VREG_VOLTAGE_1_20
#define DVI_TIMING dvi_timing_640x480p_60hz

void DVIStart(void);

#endif