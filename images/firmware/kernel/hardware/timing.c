// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      timing.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      18th January 2024
//      Reviewed :  No
//      Purpose :   Timing stuff
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

struct repeating_timer timer;

bool tick50HzHasFired = false;

// ***************************************************************************************
//
//                                  Set the tick flag
//
// 		   This is optimised out because gcc doesn't recognise side effects on extern
//
// ***************************************************************************************

#pragma GCC push_options      
#pragma GCC optimize ("O0")
static void _tmrSetTick(void) {
    tick50HzHasFired = true;                                                    // Set tick flag.    
}
#pragma GCC pop_options

// ***************************************************************************************
//
//                         Interrupt handler 50Hz Callback function
//
// ***************************************************************************************

static bool Tick50Callback(struct repeating_timer *t) {
    _tmrSetTick();                                                              // Sets the flag.
    return true;
}

// ***************************************************************************************
//
//						    Start the 50Hz timer tick interrupt
//
// ***************************************************************************************

void TMRStartTickInterrupt(void) {
    add_repeating_timer_ms(20, Tick50Callback, NULL, &timer);                   // Call above function every 20ms.
}

// ***************************************************************************************
//
//                              Read 100Hz timer
//
// ***************************************************************************************

int TMRRead(void) {
    uint32_t time32 = (uint32_t)(time_us_64() >> 10);                           // divide by 1024
    return (time32 * 210) >> 11;                                                // Error of about 0.07%
}
