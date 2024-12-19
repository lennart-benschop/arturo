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
#include "hardware/kbdcodes.h"

#include "ff.h"

#include "support/console.h"

#define KBDEvent(x,y,z) CONWriteString("%d %d %d\r",x,y,z)
void KBDSync(void);
void __time_critical_func(TASKManager)(void);
void KBDInitialise(void);
void KBDSync(void);
void STOSynchronise(void);
void THWStart(void);
uint32_t TMRRead(void);

#endif