// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		fileio.c
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		25th December 2024
//		Reviewed :	No
//		Purpose :	File I/O (FSYS Level interface)
//					This level does not check things like valid handles. It's purpose
//					is to precisely perform the required task.
//
// ***************************************************************************************
// ***************************************************************************************

#include "artsim.h"
#include "sys/stat.h"
#include "dirent.h"

// ***************************************************************************************
//
//								Current open files.
//
// ***************************************************************************************

static FILE *files[FIO_MAX_HANDLES];

// ***************************************************************************************
//
//							Reset all file objects etc.
//
// ***************************************************************************************

void FSYSInitialise(void) {
}

// ***************************************************************************************
//
//							Mep errno to our errors
//
// ***************************************************************************************

static int _FSYSMapError(void) {
	int e;
	switch(errno) {
		case 0:
			e = 0;break;
		case ENOENT:
			e = FIO_ERR_NOTFOUND;break;
		default:
			e = FIO_ERR_SYSTEM;break;
	}
	return e;
}

// ***************************************************************************************
//
//							Convert file to an internal file name.
//
// ***************************************************************************************

static char *_FSYSMapName(char *name) {
	static char buffer[256];
	mkdir("storage",0777);  													// Creates the working directory
	strcpy(buffer,"storage/");strcat(buffer,name);  							// Convert filename to working file name.
	return(buffer);
}

// ***************************************************************************************
//
//				Get file information. Fills structure if provided and no error.
//
// ***************************************************************************************

int FSYSFileInformation(char *name,FIOInfo *info) {

	DIR *f = opendir(_FSYSMapName(name));  										// Directory check
	if (f != NULL) {
		closedir(f);  															// Found, close it
		if (info != NULL) {  													// Fill in if required.
			info->length = 0;info->isDirectory = -1; 
		}
		return 0;  			
	}
	struct stat ffi;
	if (stat(_FSYSMapName(name),&ffi) != 0) return _FSYSMapError();  			// Try for file, Stat failed
	if (info != NULL) {
		info->isDirectory = false;
		info->length = info->isDirectory ? 0 : ffi.st_size;    					// Length if file, 0 for directory.
	}
	return 0;
}

// ***************************************************************************************
//
//					Create a new file, deleting any currently existing file
//
// ***************************************************************************************

int FSYSCreateFile(char *name) {
	FILE *f = fopen(_FSYSMapName(name),"w");  									// Creates an empty file.
	if (f == NULL) return _FSYSMapError();  									// Create filed.
	fclose(f);  																// Close the new file.
	return 0;
}  	

// ***************************************************************************************
//
//						Delete file, ignoring if it does not exist
//
// ***************************************************************************************

int FSYSDeleteFile(char *name) {
	if (unlink(_FSYSMapName(name)) == 0) return 0;  							// Delete worked okay.
	if (errno == ENOENT) return 0;  											// Didn't exist, that's okay.
	return _FSYSMapError();	
}

// ***************************************************************************************
//
//					Create a new directory if it does not already exist
//
// ***************************************************************************************

int FSYSCreateDirectory(char *name) {  											
	if (mkdir(_FSYSMapName(name),0777) == 0) return 0;  						// Create directory okay
	if (errno == EEXIST) return 0;  											// It already exists, not an error
	return _FSYSMapError();
}

// ***************************************************************************************
//
//				Delete directory, ignoring if it does not exist/not empty
//
// ***************************************************************************************

int FSYSDeleteDirectory(char *name) {
	if (rmdir(_FSYSMapName(name)) == 0) return 0;  								// Removal okay
	if (errno == ENOENT || errno == EEXIST || 
						errno == ENOTEMPTY || errno == ENOTDIR) return 0;  		// Not present, not empty etc.
	return _FSYSMapError();
}  											

int  	FSYSOpen(char *name);  														// Open an existing file, read or write.
int 	FSYSClose(int handle);  													// Close an open file.
int  	FSYSRead(int handle,void *data,int size);  									// Read bytes from a file.
int  	FSYSWrite(int handle,void *data,int size);  								// Write bytes to a file.

int 	FSYSEndOfFile(int handle);  												// Check EOF
int 	FSYSGetSetPosition(int handle,int newPosition);  							// Read and optionally set position.
