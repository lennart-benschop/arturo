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

void TESTShowUSBRootDirectory(void);

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

// ***************************************************************************************
//
//                          This tests the USB key is working.
//
// ***************************************************************************************

void TESTShowUSBRootDirectory(void) {
    FIOInfo fi;int e;char *s;
    char buffer[42];

    CONWriteString("\rFSYSFileInformation\r");
    s = "inline.bas";e = FSYSFileInformation(s,&fi);CONWriteString("(%d) %s %d %d ",e,s,fi.length,fi.isDirectory);
    s = "inline.xxx";e = FSYSFileInformation(s,&fi);CONWriteString("(%d) %s %d %d ",e,s,fi.length,fi.isDirectory);
    s = "aadir";e = FSYSFileInformation(s,&fi);CONWriteString("(%d) %s %d %d ",e,s,fi.length,fi.isDirectory);
    s = "abdir";e = FSYSFileInformation(s,&fi);CONWriteString("(%d) %s %d %d\r",e,s,fi.length,fi.isDirectory);

    CONWriteString("\rFSYSCreateFile/FSYSDeleteFile\r");
    e = FSYSCreateFile("newfile.1");  CONWriteString("Create %d ",e);
    e = FSYSCreateFile("newfile.2");  CONWriteString("Create %d ",e);
    e = FSYSDeleteFile("newfile.1");  CONWriteString("Delete %d ",e);
    e = FSYSDeleteFile("newfile.3");  CONWriteString("Delete %d\r",e);

    CONWriteString("\rFSYSCreateDirectory/FSYSDeleteDirectory\r");
    e = FSYSCreateDirectory("newDirectory.1");  CONWriteString("Create %d ",e);
    e = FSYSCreateDirectory("newDirectory.2");  CONWriteString("Create %d ",e);
    e = FSYSDeleteDirectory("newDirectory.1");  CONWriteString("Delete %d ",e);
    e = FSYSDeleteDirectory("newDirectory.3");  CONWriteString("Delete %d\r",e);

    CONWriteString("\rFile R/W\r");
    e = FSYSCreateFile("test.file");CONWriteString(" %d",e);
    e = FSYSOpen(0,"test.file");CONWriteString(" %d",e);
    e = FSYSWrite(0,"Hello world",5);CONWriteString(" %d",e);
    e = FSYSClose(0);CONWriteString(" %d\n",e);

    strcpy(buffer,"_________");
    e = FSYSOpen(0,"test.file");CONWriteString(" %d",e);
    e = FSYSRead(0,buffer,2);CONWriteString(" %d %s",e,buffer);    
    e = FSYSRead(0,buffer+3,3);CONWriteString(" %d %s",e,buffer);
    e = FSYSGetSetPosition(0,1);CONWriteString(" %d",e);
    e = FSYSRead(0,buffer+7,1);CONWriteString(" %d %s",e,buffer);
    e = FSYSClose(0);CONWriteString(" %d\n",e);

    CONWriteString("\rFSYSOpenDirectory\r");
    e = FSYSOpenDirectory("/");CONWriteString(" %d",e);
    while (e = FSYSReadDirectory(buffer),e == 0) CONWriteString(" %s",buffer);
    CONWriteString(" %d",e);
    e = FSYSCloseDirectory();CONWriteString(" %d",e);
}
