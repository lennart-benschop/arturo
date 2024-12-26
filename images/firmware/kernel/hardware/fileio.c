// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		fileio.c
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		21st December 2024
//		Reviewed :	No
//		Purpose :	File I/O (FSYS Level interface)
//					This level does not check things like valid handles. It's purpose
//					is to precisely perform the required task.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "ff.h"

// ***************************************************************************************
//
//								Current open files.
//
// ***************************************************************************************

static FILINFO files[FIO_MAX_HANDLES];

// ***************************************************************************************
//
//							Map FATFS error onto FIO error
//
// ***************************************************************************************

static int _FSYSMapError(FRESULT r) {
	int err;

	switch(r) {
		case FR_OK:  															// No error.
			err = FIO_OK;break;
		case FR_NO_FILE:  														// File/Path not found
		case FR_NO_PATH:
			err = FIO_ERR_NOTFOUND;break;
		case FR_INVALID_NAME:  													// Input is wrong.
		case FR_INVALID_OBJECT:
		case FR_INVALID_PARAMETER:
			err = FIO_ERR_COMMAND;break;
		default: 																// Everything else.
			err = FIO_ERR_SYSTEM;break;
			break;
	}
	return err;
}

// ***************************************************************************************
//
//							Reset all file objects etc.
//
// ***************************************************************************************

void FSYSInitialise(void) {
}

// ***************************************************************************************
//
//				Get file information. Fills structure if provided and no error.
//
// ***************************************************************************************

int FSYSFileInformation(char *name,FIOInfo *info) {
	FILINFO ffi;
	FRESULT r = f_stat(name,&ffi);  												// Read the information
	if (r == FR_OK && info != NULL) {  												// Information received.
		info->isDirectory = (ffi.fattrib & AM_DIR) != 0;  							// Directory flag
		info->length = info->isDirectory ? 0 : ffi.fsize;    						// Length if file, 0 for directory.
	}
	return _FSYSMapError(r);
}

// ***************************************************************************************
//
//					Create a new file, deleting any currently existing file
//
// ***************************************************************************************

int FSYSCreateFile(char *name) {
	FIL fRec;
	FRESULT fr = f_open(&fRec,name,FA_CREATE_ALWAYS|FA_WRITE);  				// Try to create a file
	if (fr != FR_OK) return _FSYSMapError(fr);  								// Creation failed.
	f_close(&fRec);  															// Close it.
	return 0;
}  	

// ***************************************************************************************
//
//						Delete file, ignoring if it does not exist
//
// ***************************************************************************************

int FSYSDeleteFile(char *name) {
	FRESULT fr = f_unlink(name);
	if (fr == FR_OK) return 0;  												// Delete worked okay.
	if (fr == FR_NO_FILE || fr == FR_NO_PATH) return 0;   						// Does not exist
	return _FSYSMapError(fr);	
}

// ***************************************************************************************
//
//					Create a new directory if it does not already exist
//
// ***************************************************************************************

int FSYSCreateDirectory(char *name) {  											
	FRESULT fr = f_mkdir(name);
	if (fr == FR_OK) return 0;  												// Create worked okay.
	if (fr == FR_EXIST) return 0;  												// It already exists, not an error
	return _FSYSMapError(fr);
}

// ***************************************************************************************
//
//				Delete directory, ignoring if it does not exist/not empty
//
// ***************************************************************************************

int FSYSDeleteDirectory(char *name) {
	return FSYSDeleteFile(name);  												// Same as delete file :)
}  											

