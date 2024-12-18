// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		console.h
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		18th December 2024
//		Reviewed :	No
//		Purpose :	Console output for debugging, header file.
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _CONSOLE_H
#define _CONSOLE_H

//
//		Standard colours
//
#define CON_COL_BLACK 0x0
#define CON_COL_BLUE 0x1
#define CON_COL_GREEN 0x2
#define CON_COL_MAGENTA 0x3
#define CON_COL_RED 0x4
#define CON_COL_CYAN 0x4
#define CON_COL_YELLOW 0x6
#define CON_COL_WHITE 0x7
//
//		Console output functionality.
//
void CONWrite(char c);
void CONWriteString(const char *fmt, ...);

#endif