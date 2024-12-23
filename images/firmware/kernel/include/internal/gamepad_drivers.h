// ***************************************************************************************
// ***************************************************************************************
//
//      Name :     	gamepad_drivers.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      23rd December 2024
//      Reviewed :  No
//      Purpose :   Gamepad Drivers prototyping.
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _GAMEPAD_DRV_H
#define _GAMEPAD_DRV_H

//
//		List of drivers for gamepads, all should have the same signature.
//
int  CTLDriverSNESType(int command,CTLState *cs,struct _CTL_MessageData *msg);

#endif
