// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      graphics.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      28th December 2024
//      Reviewed :  No
//      Purpose :   Graphics core test
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "gfxtest.h"

// ***************************************************************************************
//
//                      Start and run the CPU. Does not have to return.
//
// ***************************************************************************************

void ApplicationRun(void) {
    int n = 0;
    CONWriteString("%s Graphics Demo Application\r",XXX);                                          
    PrintHello();                                                                   // A library function
    //
    //      A typical 'main'
    //
    while (1) {
  
        if (KBDEscapePressed(true)) {                                               // Escaped ?
            CONWriteString("Escape !\r");
        }

        if (HASTICK50_FIRED()) {                                                    // Time to do a 50Hz tick (Don't use this for timing !)
            TICK50_RESET();                                                         // Reset the tick flag
            if (USBUpdate() == 0) return;                                           // Update USB
            KBDCheckTimer();                                                        // Check for keyboard repeat        
        }
    }
}

// ***************************************************************************************
//
//                                  Sound driver. (dummy)
//
// ***************************************************************************************

int8_t ApplicationGetChannelSample(int channel) {
    return 0; 
}