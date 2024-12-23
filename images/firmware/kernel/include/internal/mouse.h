// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      mouse.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      23rd December 2024
//      Reviewed :  No
//      Purpose :   Mouse interface header (internal)
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _MOUSEI_H
#define _MOUSEI_H

void MSEInitialise(void);
void MSEEnableMouse(void);
void MSEUpdateScrollWheel(int8_t ds);
void MSEUpdateButtonState(uint8_t bs);
void MSEOffsetPosition(int8_t dx, int8_t dy);

#endif