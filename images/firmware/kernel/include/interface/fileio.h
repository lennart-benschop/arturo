// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		fileio.h
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		21st December 2024
//		Reviewed :	No
//		Purpose :	File I/O Header
//
// ***************************************************************************************
// ***************************************************************************************


#ifndef _FILEIOI_H
#define _FILEIOI_H

#define FIO_MAX_HANDLES 	(8) 													// Maximum number of supported files.

void 	FSYSInitialise(void);

int  	FSYSOpen(char *name,char access);  											// Open an existing file, read or write.
int 	FSYSClose(int handle);  													// Close an open file.
int  	FSYSRead(int handle,void *data,int size);  									// Read bytes from a file.
int  	FSYSWrite(int handle,void *data,int size);  								// Write bytes to a file.

int 	FSYSFileExists(char *name);  												// Check if file exists.
int 	FSYSCreateFile(char *name);  												// Create an empty file of that name.
int  	FSYSCreateDirectory(char *name);  											// Create a directory of that name
int 	FSYSDeleteFile(char *name);  												// Delete a file
int  	FSYSDeleteDirectory(char *name);

#endif
