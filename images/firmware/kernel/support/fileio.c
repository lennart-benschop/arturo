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
//			A simple wrapper round FIORead() which gives some checking, returns
//			0 or a negative error code.
//
// ***************************************************************************************

int FIOReadDirectory(int h,FIOInfo *info) {  										// Simple wrapper function.
	return FIORead(h,info,0,NULL);
}

// ***************************************************************************************
//
//			Create an empty new file and opening it. Overwrites any existing file
//								Returns 0 or negative error
//
// ***************************************************************************************

int FIOOpenCreate(const char *fileName) {
	int e = FIODelete(fileName);  													// Delete if it exists
	if (e != FIO_OK) return e; 														// Some sort of error.
	return FIOOpen(fileName);
}

