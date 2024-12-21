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
    DIR di;
    FILINFO fi;
    FRESULT r = f_opendir(&di,"/");                                                 // Open root
    int done = 0;
    CONWriteString("\r");
    while (r == FR_OK && done == 0) {                                               // While more to reed (if successful)
        r = f_readdir(&di,&fi);                                                     // Read next, dump entry
        CONWriteString("%s ",fi.fname);
        done = fi.fname[0] == 0;                                                    // This marks end of read of directory
    }
    r = f_closedir(&di);
    CONWriteString("\r\r");
}

// ***************************************************************************************
//
//                      Start and run the CPU. Does not have to return.
//
// ***************************************************************************************

void TESTApplication(void) {
    int n = 0;
    CONWriteString("\rTest App\r");                                                 // Exciting title
    TESTShowUSBRootDirectory();                                                     // Dump the USB key
    //
    //      A typical 'main'
    //
    while (1) {
  
        n = KBDGetKey();                                                            // Echo any keystroke
        if (n != 0) CONWriteString("%d %c\r",n,n);

        if (HASTICK50_FIRED()) {                                                    // Time to do a 50Hz tick.
            TICK50_RESET();                                                         // Reset the tick flag
            USBUpdate();                                                            // Update USB
            KBDCheckTimer();                                                        // Check for keyboard repeat

            uint16_t x,y;                                                           // Read the mouse state.
            uint8_t b,w;    
            MSEGetState(&x,&y,&b,&w);
            //CONWriteString("Mouse:%d %d %d %d\r",x,y,b,w);                        // Uncomment to show it
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
    count++;
    if (channel == 1) {  															// Noise channel.
        return rand() & 0xFF;
    } else {
        int toggleRate = SNDGetSampleFrequency() / 440;                             // 440Hz is A
        return ((count/toggleRate) & 1) ? -127:127;
    }
}
