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

void CatUSB(void) {
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

// ***************************************************************************************
//
//										Start the kernel
//
// ***************************************************************************************

int main() {
	DVIStart();																	// Start DVI running.
	CONInitialise();  															// Initialise the console.
    CONWriteString("Arturo pre-release\r\r");                                   // Prompt string.
    KBDInitialise();                                                            // Initialise keyboard & USB system.
    KBDEvent(0,0xFF,0);                                                         // Reset the keyboard manager
    STOSynchronise();                                                           // Synchronise storage
    TMRStartTickInterrupt();
    CatUSB();
    TASKManager();

}
