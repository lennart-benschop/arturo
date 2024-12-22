// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		fileio.c
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		21st December 2024
//		Reviewed :	No
//		Purpose :	File I/O mapping.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

// ***************************************************************************************
//
//			A simple wrapper round FIORead() which gives some checking
//
// ***************************************************************************************

int FIOReadDirectory(int h,FIOInfo *info) {  										// Simple wrapper function.
	return FIORead(h,info,0,NULL);
}

// ***************************************************************************************
//
//								Create and open
//
// ***************************************************************************************

int FIOOpenCreate(const char *fileName) {
	int e = FIODelete(fileName);  													// Delete if it exists
	if (e != FIO_OK) return e; 														// Some sort of error.
	return FIOOpen(fileName);
}