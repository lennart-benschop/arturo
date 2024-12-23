// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		common.h
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		18th December 2024
//		Reviewed :	No
//		Purpose :	Common header file.
//
// ***************************************************************************************
// ***************************************************************************************

#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>

//
//		Used building for the PICO
//
#ifdef PICO
#include "ff.h"
#include "data/__config.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"
#include "hardware/watchdog.h"
#include "hardware/pwm.h"
#else
//
//		Outside that we use standard libraries.
//
#include <stdint.h>
#include <stdbool.h>
#endif

#include "interface/gamepad.h"
#include "interface/dvi_common.h"
#include "interface/miscellany.h"
#include "interface/sound.h"
#include "interface/mouse.h"
#include "interface/fileio.h"
#include "interface/usb_interface.h"

#ifdef PICO
#include "internal.h"
#endif
#include "support.h"

//
//		Note: this is a MACRO (#define) set up in config.make
//
void ARTURO_RUN_FUNCTION(void);

#endif