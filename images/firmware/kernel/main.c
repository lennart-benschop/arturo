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
    CONWriteString("Arturo pre-release %s\r\r",PICO_PLATFORM_NAME);             // Prompt string.
    SNDInitialise(false);                                                       // Start the sound system, seperate channels.
    USBInitialise();                                                            // Initialise keyboard & USB system.
    USBKeyboardEvent(0,0xFF,0);                                                 // Announce keyboard has been reset
    USBSynchronise();                                                           // Synchronise storage
    TMRStartTickInterrupt();                                                    // Start the tick interrupt.
    LOCSetLocale("gb");                                                         // GB Locale
    ARTURO_RUN_FUNCTION();                                                      // Run whatever (defined in config.make)
}
