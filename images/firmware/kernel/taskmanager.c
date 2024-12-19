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
        sleep_ms(100);
        // CONWrite(48+n);
        n = (n + 1) % 7;
        USBUpdate();
    }
}
