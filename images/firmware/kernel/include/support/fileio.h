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


#ifndef _FILEIO_H
#define _FILEIO_H

//
//		New errors. The numbers refer to FatFS errors, these are combined.
//
#define FIO_OK 				(0)  													// No error
#define FIO_ERR_SYSTEM 		(-1) 													// Internal system error
#define FIO_ERR_NOTFOUND 	(-2) 													// Directory or File not found
#define FIO_ERR_COMMAND		(-3) 													// Bad input of some sort
#define FIO_ERR_MAXFILES 	(-4) 													// Too many files open
#define FIO_ERR_HANDLE 		(-5)  													// Bad handle / not open
#define FIO_ERR_READONLY 	(-6)  													// Writing to read opened file.
#define FIO_EOF  			(1)  													// End of directory list/file value.
#define FIO_MAX_NAME_SIZE 	(32) 													// Is arbitrarily truncated to this length.

typedef struct _FIO_Information {
	char name[FIO_MAX_NAME_SIZE+1];  												// File name
	int  length;  																	// Byte size
	bool isDirectory;  																// True if directory	
} FIOInfo;

#endif
