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
//										Start the kernel
//
// ***************************************************************************************

int main() {
	DVIStart();																	// Start DVI running.
	CONInitialise();  															// Initialise the console.
    CONWriteString("Arturo pre-release\r\r");                                   // Prompt string.
    CONWriteString("Sample speed %d\r",SNDGetSampleFrequency());
    SNDInitialise();                                                            // Start the sound system.
    USBInitialise();                                                            // Initialise keyboard & USB system.
    USBKeyboardEvent(0,0xFF,0);                                                 // Announce keyboard has been reset
    USBSynchronise();                                                           // Synchronise storage
    TMRStartTickInterrupt();                                                    // Start the tick interrupt.
    TESTShowUSBRootDirectory();                                                 // Test function.
    LOCSetLocale("gb");                                                         // GB Locale
    TASKManager();                                                              // Run whatever.
}
