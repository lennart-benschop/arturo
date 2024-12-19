// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      taskmanager.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      18th December 2024
//      Reviewed :  No
//      Purpose :   The main task.
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

// ***************************************************************************************
//
//                      Start and run the CPU. Does not have to return.
//
// ***************************************************************************************

void TASKManager(void) {
    int n = 0;
    while (1) {
  
        n = KBDGetKey();
        if (n != 0) CONWriteString("%d %c\r",n,n);

        if (HASTICK50_FIRED()) {
            TICK50_RESET();
            USBUpdate();
            #ifndef HANDLE_USB_KBD_MESSAGES
            KBDCheckTimer();
            CONWrite('.');
            #endif
        }
    }
}
