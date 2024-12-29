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
    GFXPort vp;
    CONWriteString("%s Graphics Demo Application\r",XXX);                                          
    GFXPortInitialise(&vp,45,64,403,150);

    printf("%02x\n",DVIGetModeInformation()->bitPlane[0][0]);
    //
    //      A typical 'main'
    //
    while (1) {
        n++;
        GFXASetPort(&vp);
        GFXAPlot(0,0,7);GFXAPlot(vp.width-1,0,7);
        for (int y = 2;y < 240;y++) {
            // for (int i = 0;i < 640;i++) {
            //     GFXAPlot(i,y,(y+n) >> 2);
            // }            
            GFXAHorizLine(0,640,y,(y+n) >> 2);
        }
  
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