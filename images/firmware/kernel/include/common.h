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

#include "pico/stdlib.h"

#include "support.h"

#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"
#include "hardware/watchdog.h"
#include "hardware/pwm.h"
#include "hardware/gamepad.h"

#include "data/__config.h"

#include "hardware/dvi_common.h"
#include "hardware/miscellany.h"
#include "hardware/sound.h"
#include "hardware/mouse.h"
#include "hardware/fileio.h"

#include "internal.h"

//
//		Note: this is a MACRO (#define) set up in config.make
//
void ARTURO_RUN_FUNCTION(void);

#endif