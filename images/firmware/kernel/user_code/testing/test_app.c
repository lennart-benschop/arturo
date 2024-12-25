// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      test_app.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      18th December 2024
//      Reviewed :  No
//      Purpose :   Test application
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

// ***************************************************************************************
//
//                          This tests the USB key is working.
//
// ***************************************************************************************

void TESTShowUSBRootDirectory(void) {

  
}

// ***************************************************************************************
//
//                      Start and run the CPU. Does not have to return.
//
// ***************************************************************************************

void TESTApplication(void) {
    int n = 0;
    CONWriteString("\rTest App\r");                                                 // Exciting title
    TESTShowUSBRootDirectory();
    //
    //      A typical 'main'
    //
    while (1) {
  
        n = KBDGetKey();                                                            // Echo any keystroke
        if (n != 0) CONWriteString("%d %c\r",n,n);
        if (n == ' ') TESTShowUSBRootDirectory();                                  // Dump the USB key on space

        if (KBDEscapePressed(true)) {                                               // Escaped ?
            CONWriteString("Escape !\r");
        }

        if (HASTICK50_FIRED()) {                                                    // Time to do a 50Hz tick (Don't use this for timing !)
            TICK50_RESET();                                                         // Reset the tick flag
            if (USBUpdate() == 0) return;                                           // Update USB
            KBDCheckTimer();                                                        // Check for keyboard repeat
            
             int x,y,b,w;
             MSEGetState(&x,&y,&b,&w);
             if (b != 0) CONWriteString("Mouse:%d %d %d %d\r",x,y,b,w);              // Show mouse if button pressed
             
             if (CTLControllerCount() != 0) {                                        // Show controller state if anything pressed.
                CTLState *c = CTLReadController(0);
                if (c->dx != 0 || c->dy != 0 || c->a || c->b || c->x || c->y) {
                    CONWriteString("DX:%3d DY:%3d A:%d B:%d X:%d Y:%d\r",c->dx,c->dy,
                                             c->a ? 1:0,c->b ? 1:0,c->x ? 1:0,c->y ? 1:0);
                    }
                }
        }
    }
}

// ***************************************************************************************
//
//     Sound driver. The left channel (0) is a beep, the right channel (1) white noise
//
// ***************************************************************************************

static int count = 0;

int8_t TESTApplication_GetSample(int channel) {
    return 0;
    count++;
    if (channel == 0) {                                                             // Square wave on left channel.
        int toggleRate = SNDGetSampleFrequency() / (440*2);                         // 440Hz is A
        return ((count/toggleRate) & 1) ? -127:127;
    }
    if (channel == 1) {  															// Noise on right channel if supported
         return rand() & 0xFF;
    } 
    return 0; 
}
