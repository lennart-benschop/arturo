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
void TestScrollAndRect(void);
void TestEllipse(void);
void TestTriangles(void);

// ***************************************************************************************
//
//                      Start and run the CPU. Does not have to return.
//
// ***************************************************************************************

static GFXPort vp;

void ApplicationRun(void) {
    int n = 0;
    // GFXSetMode(DVI_MODE_640_240_8);
    CONWriteString("%s Graphics Demo Application\r",XXX);                                          
    GFXPortInitialise(&vp,45,64,553,150);
    //
    //      A typical 'main'
    //
    GFXDrawCharacter(&vp,30,50,'?',6,4);
    while (1) {
        n++;
        GFXFillTriangle(&vp,50,20,10,100,320,140,1);
        GFXFrameTriangle(&vp,50,20,10,100,320,140,7);
        // TestCodeHorizontalLines();
        // TestCodeRandomLines();
        // TestCodeVerticalLines();
        // TestScrollAndRect();
        // TestEllipse();
             TestTriangles();

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
        GFXLine(&vp,2,y,640,y,(y+ctr) >> 2);
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
        GFXLine(&vp,rand() % 640,rand() % 240,rand() %640,rand() % 240,rand()%7+1);
    }
}

// ***************************************************************************************
//
//                                  Vertical line test code
//
// ***************************************************************************************

void TestCodeVerticalLines(void) {
    for (int x = 2;x < 640;x++) {
        GFXLine(&vp,x,2,x,300,(x+ctr) >> 2);
    }
    ctr++;
}

// ***************************************************************************************
//
//                                  Tests scrolling and frame/rect
//
// ***************************************************************************************

static int nextDraw = 0;

void TestScrollAndRect(void) {
    if (TMRRead() < nextDraw) return;
    nextDraw = TMRRead()+3;
    GFXScrollPort(&vp,0,0);
    GFXPlot(&vp,0,0,7);GFXPlot(&vp,vp.width-1,0,7);GFXPlot(&vp,0,vp.height-1,7);
    int s = -(ctr % 100);
    GFXScrollPort(&vp,s,s);
    GFXFillRect(&vp,9,9,301,101,ctr & 7);
    ctr++;
}

// ***************************************************************************************
//
//                                  Ellipse test
//
// ***************************************************************************************

void TestEllipse(void) {
    GFXFillEllipse(&vp,20,30,300,130,3);
    GFXFrameEllipse(&vp,20,30,300,130,5);
    GFXFrameRect(&vp,20,30,300,130,1);
}

// ***************************************************************************************
//
//                                  Triangle test
//
// ***************************************************************************************

void TestTriangles(void) {
    GFXFillTriangle(&vp,rand()%640,rand()%240,rand()%640,rand()%240,rand()%640,rand()%240,rand() & 7);
    GFXFrameTriangle(&vp,rand()%640,rand()%240,rand()%640,rand()%240,rand()%640,rand()%240,rand() & 7);
}