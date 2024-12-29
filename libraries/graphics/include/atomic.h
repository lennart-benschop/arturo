// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      atomic.h
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      29th December 2024
//      Reviewed :  No
//      Purpose :   Graphics core header (atomic)
//
// ***************************************************************************************
// ***************************************************************************************

#pragma once

#define XXX "Hello"

void GFXASetPort(GFXPort *vp);
void GFXAPlot(int x,int y,int colour);
void GFXAHorizLine(int x1,int x2,int y,int colour);
void GFXAVertLine(int x,int y1,int y2,int colour);
void GFXALine(int x0, int y0, int x1, int y1,int colour);

void GFXAUp(void);
void GFXADown(void);
void GFXALeft(void);
void GFXARight(void);
