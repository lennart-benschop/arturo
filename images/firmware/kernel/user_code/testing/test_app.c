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
    #ifdef PICO

    int h,e,count;
    FIOInfo info;
    char *s;

    e = FIOCreateDirectory("aadir");
    CONWriteString("Create Dir %d\r",e);

    h = FIOOpenDirectory("/");
    CONWriteString("Open Dir %d\r",h);

    while (e = FIORead(h,&info,0,NULL),e == 0) {
        CONWriteString("[%s %d %d] ",info.name,info.length,info.isDirectory ? 1 : 0);
    }
    CONWriteString("\rRead Dir ended %d\r",e);
    e = FIOClose(h);
    CONWriteString("\rClose Dir %d\r",e);

    h = FIOOpenCreate("inline.bsc");
    e = FIOWrite(h,"Hello World",5); 
    CONWriteString("\rWriting %d\r",e);    
    FIOClose(h);

    h = FIOOpenRead("inline.bsc");
    CONWriteString("Open %d\r",h);
    FIOSetPosition(h,2);
    count = FIOGetPosition(h);
    CONWriteString("Position : %d\r",count);
    while (FIOEndOfFile(h) == 0) {
        char c = 0;
        e = FIORead(h,&c,1,&count);
        CONWriteString("%d.%c ",c,c);
    }
    e = FIOClose(h);
    CONWriteString("Close %d\r",e);

    char buffer[12];
    h = FIOOpen("inline.bsc");
    e = FIORead(h,buffer,10,&count);
    CONWriteString("Read 10 %d Read %d\r",e,count);
    e = FIOClose(h);
    for (int i = 0;i < 10;i++) CONWriteString("%d ",buffer[i]);
    CONWriteString("\n");

    s = "aadir";CONWriteString("%s %d\r",s,FIOExists(s));
    s = "abdir";CONWriteString("%s %d\r",s,FIOExists(s));
    s = "binary.bas";CONWriteString("%s %d\r",s,FIOExists(s));

    #endif
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

        if (HASTICK50_FIRED()) {                                                    // Time to do a 50Hz tick.
            TICK50_RESET();                                                         // Reset the tick flag
            if (USBUpdate() == 0) return;                                              // Update USB
            KBDCheckTimer();                                                        // Check for keyboard repeat
            
         int x,y,b,w;
         //MSEGetState(&x,&y,&b,&w);
         //CONWriteString("Mouse:%d %d %d %d\r",x,y,b,w);                        // Uncomment to show it
         if (CTLControllerCount() != 0) {                                        // Show controller state if anything pressed.
            CTLState *c = CTLReadController(0);
            if (c->dx != 0 || c->dy != 0 || c->a || c->b || c->x || c->y) {
                CONWriteString("DX:%3d DY:%3d A:%d B:%d X:%d Y:%d\r",c->dx,c->dy,
                                         c->a ? 1:0,c->b ? 1:0,c->x ? 1:0,c->y ? 1:0);
                }
            }
            //CONWrite('_');
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
//     count++;
//     if (channel == 1) {  															// Noise channel.
         return rand() & 0xFF;
//     } else {
//         int toggleRate = SNDGetSampleFrequency() / 440;                             // 440Hz is A
//         return ((count/toggleRate) & 1) ? -127:127;
//     }
}
