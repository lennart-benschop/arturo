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

#include "ff.h" // TODO:Remove

//
//		New errors. The numbers refer to FatFS errors, these are combined.
//
#define FIO_OK 				(0)  													// No error
#define FIO_ERR_SYSTEM 		(-1) 													// Internal system error
#define FIO_ERR_NOTFOUND 	(-2) 													// Directory or File not found
#define FIO_ERR_COMMAND		(-3) 													// Bad input of some sort
#define FIO_ERR_MAXFILES 	(-4) 													// Too many files open

void FIOInitialise(void);

int FIOReadError(int fr);

int FIOOpenFileRead(const char *fileName);
int FIOOpenFileWrite(const char *fileName);
int FIOOpenDirectory(const char *dirName);

int FIORead(int fr,void *data,int *pSize,int *pReadCount);
int FIOWrite(int fr,void *data,int *pSize);

int FIOEndOfFile(int fr);

int FIOClose(int fr);

#endif