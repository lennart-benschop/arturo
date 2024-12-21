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

#include "ff.h"

// ***************************************************************************************
//
//							This represents a single dir/file object
//
// ***************************************************************************************

struct _FIO_Object {
	bool isInUse;  																	// true if currently in use
	int error;  																	// current error, if any,0 is none.
	bool isDir;  																	// true if this is a directory.

	FRESULT fatfsError;  															// Recorded fatFS error, debugging purposes only.
	FILINFO fileInfo;  																// file information (for reading directories)
	DIR dirHandle;   																// directory handle
	FIL fileHandle;  																// file handle
};

// ***************************************************************************************
//
//					This structure represents the dir/file set.
//
// ***************************************************************************************

#define FIO_MAX_HANDLES 	(8) 													// Maximum number of supported files.

static struct _FIO_Object file[FIO_MAX_HANDLES]; 

// ***************************************************************************************
//
//							Reset all file objects
//
// ***************************************************************************************

void FIOInitialise(void) {
	for (int i = 0;i < FIO_MAX_HANDLES;i++) file[i].isInUse = false; 				// Clear all files to no longer in use.
}

