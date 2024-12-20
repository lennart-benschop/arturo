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
    FRESULT r = f_opendir(&di,"/");
    int done = 0;
    CONWriteString("\r");
    while (r == FR_OK && done == 0) {
        r = f_readdir(&di,&fi);
        CONWriteString("%s ",fi.fname);
        done = fi.fname[0] == 0;
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
    CONWriteString("\rTest App\r");
    TESTShowUSBRootDirectory();                                               
    while (1) {
  
        n = KBDGetKey();
        if (n != 0) CONWriteString("%d %c\r",n,n);

        if (HASTICK50_FIRED()) {
            TICK50_RESET();
            USBUpdate();
            KBDCheckTimer();
            CONWrite('.');
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
