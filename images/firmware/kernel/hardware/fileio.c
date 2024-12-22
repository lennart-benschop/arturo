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

// ***************************************************************************************
//
//							Handle errors, remapped and simplified.
//
// ***************************************************************************************

static void _FIOError(int h,FRESULT r) {
	file[h].error = FIO_OK;  														// Set to ok, save fatfs result 
	file[h].fatfsError = r; 
	if (r != FR_OK) {  																// Error occurred, remap
		switch(r) {
			case FR_NO_FILE:  														// File/Path not found
			case FR_NO_PATH:
				file[h].error = FIO_ERR_NOTFOUND;break;
			case FR_INVALID_NAME:  													// Input is wrong.
			case FR_INVALID_OBJECT:
			case FR_INVALID_PARAMETER:
				file[h].error = FIO_ERR_COMMAND;break;
			default:  																// Everything else, mostly tech failings.
				file[h].error = FIO_ERR_SYSTEM;break;
		}
	}
}

// ***************************************************************************************
//
//							Open File/Directory
//
// ***************************************************************************************

static int _FIOOpenGeneral(const char *name,char mode,bool isDirectory);

int FIOOpenFileRead(const char *fileName) {
	return _FIOOpenGeneral(fileName,'R',false);
}
int FIOOpenFileWrite(const char *fileName) {
	return _FIOOpenGeneral(fileName,'W',false);
}
int FIOOpenDirectory(const char *dirName) {
	return _FIOOpenGeneral(dirName,'R',true);
}

static int _FIOOpenGeneral(const char *name,char mode,bool isDirectory) {
	int h = -1;  																	// Identify a file structure to use.
	for (int i = 0;i < FIO_MAX_HANDLES;i++) {
		if (!file[i].isInUse) h = i;
	}
	if (h < 0) return FIO_ERR_MAXFILES;  											// Too many files open.

	if (isDirectory) {  															// Open directory
		file[h].isDir = true;  														// It's a directory.
		_FIOError(h,f_opendir(&file[h].dirHandle,name)); 							// Try to open directory, process the error		
	} else {
		file[h].isDir = false;  													// It's a file.
		_FIOError(h,f_open(&file[h].fileHandle,name,(mode == 'R') ? 				// Try to open file, process the error.
											FA_READ: FA_CREATE_ALWAYS | FA_READ | FA_WRITE));

	}
	if (file[h].error == FIO_OK) file[h].isInUse = true;  							// Mark as in use if open went okay.
	return file[h].error < 0 ? file[h].error : h; 									// Return the error state or the handle.
}

// ***************************************************************************************
//
//									Close file object
//
// ***************************************************************************************

int FIOClose(int h) {
	// TODO: Handle check
	if (file[h].isInUse) {  														// File in use.		
		if (file[h].isDir) {  														// Close directory/file.
			f_closedir(&file[h].dirHandle);
		} else {
			f_close(&file[h].fileHandle);
		}
		file[h].isInUse = false;   													// No longer in use
	}
	return FIO_OK;
}
