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

#ifndef _MISCELLANY_H
#define _MISCELLANY_H

//
//		Timer/Interrupt functions.
//
int TMRRead(void);
//
//		Set every 20ms
//
extern bool tick50HzHasFired;
//
//		Macros for detecting the tick and resetting it.
//
#define HASTICK50_FIRED()	(tick50HzHasFired)
#define TICK50_RESET() 		{ tick50HzHasFired = false; }

#endif