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

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"
#include "hardware/watchdog.h"

#include "hardware/dvi_common.h"
#include "hardware/kbd_codes.h"
#include "hardware/usb_interface.h"
#include "hardware/miscellany.h"

#include "ff.h"

#include "support/console.h"
#include "support/miscellany.h"

void TASKManager(void);



#endif