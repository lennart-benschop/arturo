// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		miscellany.h
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		18th December 2024
//		Reviewed :	No
//		Purpose :	Odds and sods
//
// ***************************************************************************************
// ***************************************************************************************

#pragma once

uint8_t LOCLocaleMapping(uint8_t asciiCode,uint8_t keyCode,uint8_t modifiers);
void LOCSetLocale(char *loc);

void LOGODrawTitle(void);
void LOGODraw(int x,int y,int w,int h,const uint8_t *pixelData,int fgr,int bgr);
void LOGOTextHeader(void);
