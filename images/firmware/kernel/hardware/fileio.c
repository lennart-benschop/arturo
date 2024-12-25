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


int  	FSYSOpen(char *name,char access);  											// Open an existing file, read or write.
int 	FSYSClose(int handle);  													// Close an open file.
int  	FSYSRead(int handle,void *data,int size);  									// Read bytes from a file.
int  	FSYSWrite(int handle,void *data,int size);  								// Write bytes to a file.

int 	FSYSFileExists(char *name); 												// Check if file exists.
int 	FSYSCreateFile(char *name);  												// Create an empty file of that name.
int 	FSYSDeleteFile(char *name);  												// Delete a file

int  	FSYSCreateDirectory(char *name);  											// Create a directory of that name
int  	FSYSDeleteDirectory(char *name);  											// Delete a directory of that name