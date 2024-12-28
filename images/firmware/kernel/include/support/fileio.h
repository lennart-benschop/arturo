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

#pragma once

void FIOInitialise(void);

int FIOOpen(char *fileName);
int FIOClose(int handle);
int FIORead(int handle,void *data,int size); 
int FIOWrite(int handle,void *data,int size);
int FIOGetSetPosition(int handle,int newPosition); 

int FIOCreateFile(char *fileName);
int FIOCreateDirectory(char *fileName);
int FIODeleteFile(char *fileName);
int FIODeleteDirectory(char *fileName);
int FIOFileInformation(char *name,FIOInfo *info); 								
int FIOChangeDirectory(char *directory);

int FIOOpenDirectory(char *directory);  		
int FIOReadDirectory(int handle,char *fileName); 
int FIOCloseDirectory(int handle); 

