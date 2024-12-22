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
//							This represents a single dir/file object
//
// ***************************************************************************************

struct _FIO_Object {
	bool isInUse;  																	// true if currently in use
	int error;  																	// current error, if any,0 is none.
	bool isDir;  																	// true if this is a directory.
	bool isReadOnly;  																// true if read only

	FRESULT fatfsError;  															// Recorded fatFS error, debugging purposes only.
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

#define HANDLE_VALID(h) ((h) >= 0 && (h) < FIO_MAX_HANDLES)  						// Handle is a valid one (doesn't check in use)
#define HANDLE_VALID_OPEN(h) (HANDLE_VALID(h) && file[h].isInUse) 					// Handle is valid *and* open.

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
//							Map FATFS error onto FIO error
//
// ***************************************************************************************

static int _FIO_MapError(FRESULT r) {
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
//							Handle errors, remapped and simplified.
//
// ***************************************************************************************

static void _FIOError(int h,FRESULT r) {
	file[h].error = FIO_OK;  														// Set to ok, save fatfs result 
	file[h].fatfsError = r; 
	if (r != FR_OK) {  																// Error occurred, remap
		file[h].error = _FIO_MapError(r);
	}
}

// ***************************************************************************************
//
//							Open File/Directory
//
// ***************************************************************************************

static int _FIOOpenGeneral(const char *name,char mode,bool isDirectory);

int FIOOpen(const char *fileName) {
	return _FIOOpenGeneral(fileName,'W',false);
}
int FIOOpenRead(const char *fileName) {
	return _FIOOpenGeneral(fileName,'R',false);
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

	file[h].isReadOnly = (mode == 'R');  											// Set read-only flag.

	if (isDirectory) {  															// Open directory
		file[h].isDir = true;  														// It's a directory.
		_FIOError(h,f_opendir(&file[h].dirHandle,name)); 							// Try to open directory, process the error	
		CONWriteString("Trying to open %s returned %d\r",name,file[h].fatfsError);	
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
	if (!HANDLE_VALID(h)) return FIO_ERR_HANDLE;  									// Check handle legal
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

// ***************************************************************************************
//
//		Read Object to location, size bytes. For directories the last two parameters
//		are ignored and data must point to a FIOInfo structure.
//
// ***************************************************************************************

int FIORead(int h,void *data,int size,int *pReadCount) {

	if (!HANDLE_VALID_OPEN(h)) return FIO_ERR_HANDLE;  								// Check handle legal

	if (file[h].isDir) {  															// Directory reading code.
		FILINFO fInfo;  															// Internal FATFS structure.
		_FIOError(h,f_readdir(&file[h].dirHandle,&fInfo));  						// Read next and error check.
		if (file[h].error != FIO_OK) return file[h].error;   						// The read directory failed.
		if (fInfo.fname[0] == 0) return FIO_EOF;  									// Read all the directory, return EOF.
		
		FIOInfo *fOut = (FIOInfo *)data;  											// Where it goes.
		fInfo.fname[FIO_MAX_NAME_SIZE] = '\0';  									// Truncate the name to the max length allowed.
		strcpy(fOut->name,fInfo.fname);
		fOut->length = fInfo.fsize;  												// Set length, directory 
		fOut->isDirectory = (fInfo.fattrib & AM_DIR) != 0;
	} else {  																		// File reading code.
		UINT bytesRead;
		if (pReadCount != 0) *pReadCount = 0;  										// Zero bytes read in case of error.
		_FIOError(h,f_read(&file[h].fileHandle,data,size,&bytesRead));  			// Do the actual read.
		if (file[h].error != FIO_OK) return file[h].error;  						// Read failed.
		if (pReadCount != NULL) *pReadCount = bytesRead;   							// If read count provided, update that value.
	}
	return FIO_OK;
}

// ***************************************************************************************
//
//								Write data to file.
//
// ***************************************************************************************

int FIOWrite(int h,void *data,int size) {
	if (!HANDLE_VALID_OPEN(h)) return FIO_ERR_HANDLE;  								// Check handle legal
	if (file[h].isDir) return FIO_ERR_COMMAND;  									// Files only.
	if (file[h].isReadOnly) return FIO_ERR_READONLY;  								// Opened in read mode.

	UINT bytesWritten;	
	_FIOError(h,f_write(&file[h].fileHandle,data,size,&bytesWritten));  			// Do the actual write
	if (file[h].error != FIO_OK) return file[h].error;  							// Write failed.	
	if (bytesWritten != size) return FIO_ERR_SYSTEM;  								// Wrote insufficient data out.
	return FIO_OK;
}

// ***************************************************************************************
//
//							Check if file is at the end
//
// ***************************************************************************************

int FIOEndOfFile(int h) {
	if (!HANDLE_VALID_OPEN(h)) return FIO_ERR_HANDLE;  								// Check handle legal
	if (file[h].isDir) return FIO_ERR_COMMAND;  									// Files only.
	return (f_eof(&file[h].fileHandle) ? FIO_EOF : FIO_OK);  						// Return EOF or OK accordingly.
}

// ***************************************************************************************
//
//					Create directory, ignore already exists
//
// ***************************************************************************************

int FIOCreateDirectory(const char *dirName) {
	FRESULT err = f_mkdir(dirName);  												// Try to create directory
	if (err == FR_EXIST) err = FR_OK;  												// Ignore already exist errors.
	return _FIO_MapError(err);
}

// ***************************************************************************************
//
//						Check file or directory exists
//
// ***************************************************************************************

int FIOExists(const char *name) {
	FILINFO fInfo;  																// Get the object status.
	FRESULT fr = f_stat(name,&fInfo); 
	return _FIO_MapError(fr);  														// Return mapped error.
}

// ***************************************************************************************
//
//					 Delete file or directory, if it exists
//
// ***************************************************************************************

int FIODelete(const char *name) {
	FRESULT fr = f_unlink(name);  													// Try to delete it.
	if (fr == FR_NO_FILE || fr == FR_NO_PATH) return FIO_OK;  						// Doesn't matter if it doesn't exist
	return _FIO_MapError(fr);  														// Return mapped error.
}

// ***************************************************************************************
//
//					 			Set the file position
//
// ***************************************************************************************

int FIOSetPosition(int h,int pos) {
	if (!HANDLE_VALID_OPEN(h)) return FIO_ERR_HANDLE;  								// Check handle legal
	if (file[h].isDir) return FIO_ERR_COMMAND;  									// Files only.
	_FIOError(h,f_lseek(&file[h].fileHandle,pos));  								// Move the position.
	if (file[h].error != FIO_OK) return file[h].error;  							// Failed
	return 0;
}

// ***************************************************************************************
//
//					 			Get the file position
//
// ***************************************************************************************

int FIOGetPosition(int h) {
	if (!HANDLE_VALID_OPEN(h)) return FIO_ERR_HANDLE;  								// Check handle legal
	if (file[h].isDir) return FIO_ERR_COMMAND;  									// Files only.
	return f_tell(&file[h].fileHandle);												// Read the position.
}

