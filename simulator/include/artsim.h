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
#include <unistd.h> 
#include <sys/stat.h>
#include <errno.h>

#define AS_SCALE 	(2)

void TESTApplication(void);

void SYSOpen(bool muteSound);
int SYSPollUpdate(void);
void SYSClose(void);
void SYSRectangle(SDL_Rect *rc,int colour);

void RNDRender(SDL_Surface *surface);
int TMRRead(void);
void KBDProcessEvent(int scanCode,int modifiers,bool isDown);

void CTLFindControllers(void);

#define SNDGetNextSample TESTApplication_GetSample

int8_t SNDGetNextSample(int channel);

void SOUNDOpen(void);
void SOUNDClose(void);
void SOUNDPlay(void);
void SOUNDStop(void);
#endif