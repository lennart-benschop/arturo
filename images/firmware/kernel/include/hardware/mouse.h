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
void MSESetPosition(uint16_t x, uint16_t y);
void MSEOffsetPosition(int8_t dx, int8_t dy);
void MSEGetState(uint16_t *pX, uint16_t *pY, uint8_t *pButtonState, uint8_t *pScrollWheelState);

#endif
