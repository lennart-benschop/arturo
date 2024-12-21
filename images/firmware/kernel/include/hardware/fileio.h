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

#include "ff.h"

//
//		New errors. The numbers refer to FatFS errors, these are combined.
//
#define FIO_OK 				(0)  		/* No error 0 */
#define FIO_ERR_SYSTEM 		(0x100) 	/* Internal system error 1,2,3,7,8,10,11,12,13,14,15,16,17,18 */
#define FIO_ERR_NOTFOUND 	(0x101) 	/* Directory or File not found 4,5 */
#define FIO_ERR_COMMAND		(0x102) 	/* Bad input of some sort 6,9,19 */

typedef int FIORef;

void FIOInitialise(void);

int FIOReadError(FIORef fr);

int FIOOpenFileRead(const char *fileName,FIORef *fr);
int FIOOpenFileWrite(const char *fileName,FIORef *fr);
int FIOOpenDirectory(const char *dirName,FIORef *fr);

int FIORead(FIORef fr,void *data,int *pSize,int *pReadCount);
int FIOWrite(FIORef fr,void *data,int *pSize);

int FIOEndOfFile(FIORef fr);

int FIOClose(FIORef fr);

#endif