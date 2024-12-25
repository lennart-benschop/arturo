// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      artsim.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      23rd December 2024
//      Reviewed :  No
//      Purpose :   Artsim specific includes.
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _ARTSIM_H
#define _ARTSIM_H

#include <common.h>
#include <SDL.h>

#define AS_SCALE 	(2)

void TESTApplication(void);

void SYSOpen(void);
int SYSPollUpdate(void);
void SYSClose(void);
void SYSRectangle(SDL_Rect *rc,int colour);

void RNDRender(SDL_Surface *surface);
int TMRRead(void);
void KBDProcessEvent(int scanCode,int modifiers,bool isDown);

void CTLFindControllers(void);

#endif