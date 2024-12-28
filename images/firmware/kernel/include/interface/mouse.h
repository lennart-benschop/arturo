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

#pragma once

bool MSEMousePresent(void);
void MSESetPosition(int x, int y);
void MSEGetState(int *pX, int *pY, int *pButtonState, int *pScrollWheelState);

//
//		Exposed for simulator.
//
void MSEInitialise(void);
void MSEEnableMouse(void);
void MSEUpdateScrollWheel(int8_t ds);
void MSEUpdateButtonState(uint8_t bs);
void MSEOffsetPosition(int8_t dx, int8_t dy);


