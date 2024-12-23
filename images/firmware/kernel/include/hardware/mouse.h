// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      mouse.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      20th December 2024
//      Reviewed :  No
//      Purpose :   Mouse interface header
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _MOUSE_H
#define _MOUSE_H

bool MSEMousePresent(void);
void MSESetPosition(int x, int y);
void MSEGetState(int *pX, int *pY, int *pButtonState, int *pScrollWheelState);

#endif
