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

static FIL files[FIO_MAX_HANDLES];

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
	return FIO_OK;
}  	

// ***************************************************************************************
//
//						Delete file, ignoring if it does not exist
//
// ***************************************************************************************

int FSYSDeleteFile(char *name) {
	FRESULT fr = f_unlink(name);
	if (fr == FR_OK) return FIO_OK;  											// Delete worked okay.
	if (fr == FR_NO_FILE || fr == FR_NO_PATH) return FIO_OK;  					// Does not exist
	return _FSYSMapError(fr);	
}

// ***************************************************************************************
//
//					Create a new directory if it does not already exist
//
// ***************************************************************************************

int FSYSCreateDirectory(char *name) {  											
	FRESULT fr = f_mkdir(name);
	if (fr == FR_OK) return FIO_OK;  											// Create worked okay.
	if (fr == FR_EXIST) return FIO_OK; 											// It already exists, not an error
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

// ***************************************************************************************
//
//							Open File in R/W mode, rewind to start
//
// ***************************************************************************************

int FSYSOpen(int handle,char *name) {
	FRESULT fr = f_open(&files[handle],name,FA_READ|FA_WRITE|FA_OPEN_EXISTING);  	// Open read/write										
	if (fr != FR_OK) return _FSYSMapError(fr);  									// Error occurred of some sort.
	fr = f_lseek(&files[handle],0);   												// Rewind
	return _FSYSMapError(fr);
};

// ***************************************************************************************
//
//										Close the file
//
// ***************************************************************************************

int FSYSClose(int handle) {
	return _FSYSMapError(f_close(&files[handle])); 			 						// Close file.
}

// ***************************************************************************************
//
//				Read data from the file. Returns error or # of bytes read.
//
// ***************************************************************************************

int FSYSRead(int handle,void *data,int size) {
	UINT count;
	FRESULT fr = f_read(&files[handle],data,size,&count); 							// Attempt to read data from file.
	if (fr != FR_OK) return _FSYSMapError(fr);  									// Error occurred of some sort.
	return (int)count;  															// Bytes read.
}

// ***************************************************************************************
//
//								Write data to the file. 
//
// ***************************************************************************************

int FSYSWrite(int handle,void *data,int size) {
	UINT count;
	FRESULT fr = f_write(&files[handle],data,size,&count); 							// Attempt to write data to file.
	if (fr != FR_OK) return _FSYSMapError(fr);  									// Error occurred of some sort.
	if (count != size) return FIO_ERR_SYSTEM;  										// Write did not complete.
	return FIO_OK;  																// It's okay.
}

// ***************************************************************************************
//
//		Check end of file. Return -ve on error, 0 if more data, +ve if eof
//
// ***************************************************************************************

int FSYSEndOfFile(int handle) {
	return f_eof(&files[handle]) ? FIO_EOF : FIO_OK;  								// Seems not to be able to error :)
}

// ***************************************************************************************
//
//				Returns current position, and sets new position if >= 0
//
// ***************************************************************************************

int FSYSGetSetPosition(int handle,int newPosition) {
	int current = f_tell(&files[handle]);  											// Where are we now (cannot error in fatfs)
	if (newPosition >= 0) {  														// Moving ?
		FRESULT fr = f_lseek(&files[handle],newPosition);  							// Move it and check
		if (fr != FR_OK) return _FSYSMapError(fr);
	}
	return current;
}  	
