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
void TMRStartTickInterrupt(void);
uint32_t TMRRead(void);
//
extern bool tick50HzHasFired;

#define TICK50_RESET() 		{ tick50HzHasFired = false; }
#define HASTICK50_FIRED()	(tick50HzHasFired)

#endif