// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		main.c
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		18th December 2024
//		Reviewed :	No
//		Purpose :	Main Program.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

// ***************************************************************************************
//
//                                  USB test function
//
// ***************************************************************************************

void TESTShowUSBRootDirectory(void) {
    DIR di;
    FILINFO fi;
    FRESULT r = f_opendir(&di,"/");
    int done = 0;
    CONWriteString("\r");
    while (r == FR_OK && done == 0) {
        r = f_readdir(&di,&fi);
        CONWriteString("%s ",fi.fname);
        done = fi.fname[0] == 0;
    }
    r = f_closedir(&di);
    CONWriteString("\r\r");
}
