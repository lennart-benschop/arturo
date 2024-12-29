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

void TestCodeHorizontalLines(void);
void TestCodeRandomLines(void);
void TestCodeVerticalLines(void);

// ***************************************************************************************
//
//                      Start and run the CPU. Does not have to return.
//
// ***************************************************************************************

void ApplicationRun(void) {
    int n = 0;
    DVISetMode(DVI_MODE_320_240_8);
    GFXPort vp;
    CONWriteString("%s Graphics Demo Application\r",XXX);                                          

    GFXPortInitialise(&vp,45,64,253,150);
    //
    //      A typical 'main'
    //
    while (1) {
        n++;
        GFXASetPort(&vp);
        GFXAPlot(0,0,7);GFXAPlot(vp.width-1,0,7);GFXAPlot(0,vp.height-1,7);
        // TestCodeHorizontalLines();
        // TestCodeRandomLines();
        TestCodeVerticalLines();
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

// ***************************************************************************************
//
//                                  Horizontal line test code
//
// ***************************************************************************************

static int ctr = 0;

void TestCodeHorizontalLines(void) {
    for (int y = 2;y < 240;y++) {
        GFXAHorizLine(2,640,y,(y+ctr) >> 2);
    }
    ctr++;
}

// ***************************************************************************************
//
//                                  Random line test code
//
// ***************************************************************************************

void TestCodeRandomLines(void) {
    for (int i = 0;i < 10;i++) {
        GFXALine(rand() % 640,rand() % 240,rand() %640,rand() % 240,rand()%7+1);
    }
}

// ***************************************************************************************
//
//                                  Vertical line test code
//
// ***************************************************************************************

void TestCodeVerticalLines(void) {
    for (int x = 0;x < 640;x++) {
        GFXAVertLine(x,2,300,(x+ctr) >> 2);
    }
    ctr++;
}