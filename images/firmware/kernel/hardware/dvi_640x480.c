// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		dvi_640x480.c
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		18th December 2024
//		Reviewed :	No
//		Purpose :	DVI Driver 640x480 frame size. 
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"

#include "dvi.h"
#include "dvi_serialiser.h"
#include "common_dvi_pin_configs.h"
#include "tmds_encode.h"

#include "hardware/dvi_common.h"

// ***************************************************************************************
//
//						Frame buffer, these are 3 planar bitmaps.
//
// ***************************************************************************************

#define FRAME_WIDTH 640  															// Not the *pixels*, it's the display setting.
#define FRAME_HEIGHT 480

#define PLANE_SIZE_BYTES (FRAME_WIDTH * FRAME_HEIGHT / 8)
static uint8_t framebuf[3 * PLANE_SIZE_BYTES];

struct dvi_inst dvi0;																// PicoDVI structure
struct DVIModeInformation dvi_modeInfo;  											// Mode information structure.

// ***************************************************************************************
//
//									Get mode information
//
// ***************************************************************************************

struct DVIModeInformation *DVIGetModeInformation(void) {
	return &dvi_modeInfo;
}

// ***************************************************************************************
//
//									Set current mode
//
// ***************************************************************************************

void DVISetMode(int mode) {
	dvi_modeInfo.mode = mode;  														// Record mode
	dvi_modeInfo.userMemory = NULL;   												// Common defaults.
	dvi_modeInfo.userMemorySize = 0;		
	switch(mode) {
		case DVI_MODE_640_480_8:
			dvi_modeInfo.width = FRAME_WIDTH;
			dvi_modeInfo.height = FRAME_HEIGHT;
			dvi_modeInfo.bitPlaneCount = 3;
			dvi_modeInfo.bitPlaneSize = PLANE_SIZE_BYTES;
			dvi_modeInfo.bitPlaneDepth = 1;
			for (int i = 0;i <dvi_modeInfo.bitPlaneCount;i++)
				dvi_modeInfo.bitPlane[i] = framebuf + PLANE_SIZE_BYTES * i;
			break;
		default:
			dvi_modeInfo.mode = -1;  												// Failed.
	}
}

// ***************************************************************************************
//
//									The main line renderer
//
// ***************************************************************************************

	uint8_t buffer[640];
	
void __not_in_flash("main") dvi_core1_main() {
	const uint red_msb   = 7;
	const uint red_lsb   = 5;
	const uint green_msb = 4;
	const uint green_lsb = 2;
	const uint blue_msb  = 1;
	const uint blue_lsb  = 0;

	uint pixwidth = dvi0.timing->h_active_pixels;

	uint32_t *tmdsbuf;
	dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
	dvi_start(&dvi0);
	uint y = -1;



	while (true) {
			y = (y + 1) % FRAME_HEIGHT;
			switch(dvi_modeInfo.mode) {
				//
				//		Mode 0 is 640x480x8 colours as 3 bitplanes.
				//
				case DVI_MODE_640_480_8:
					queue_remove_blocking_u32(&dvi0.q_tmds_free, &tmdsbuf);
					for (uint component = 0; component < 3; ++component) {
						tmds_encode_1bpp(
							(const uint32_t*)&framebuf[y * FRAME_WIDTH / 8 + component * PLANE_SIZE_BYTES],
							tmdsbuf + (2-component) * FRAME_WIDTH / DVI_SYMBOLS_PER_WORD,  	// The (2-x) here makes it BGR Acordn standard
							FRAME_WIDTH
						);
					}
					queue_add_blocking_u32(&dvi0.q_tmds_valid, &tmdsbuf);
					break;
				//
				//		Mode 1 is 340x240x256 colours in byte format.
				//
				case DVI_MODE_320_240_256:					
					queue_remove_blocking_u32(&dvi0.q_tmds_free, &tmdsbuf);
					// NB the scanline buffers are half-resolution!
					for (int i = 0;i < 640;i++) buffer[i] = y;
					tmds_encode_data_channel_8bpp((const uint32_t *)buffer, tmdsbuf, pixwidth / 2, blue_msb, blue_lsb);
					tmds_encode_data_channel_8bpp((const uint32_t *)buffer, tmdsbuf + pixwidth, pixwidth / 2, green_msb, green_lsb);
					tmds_encode_data_channel_8bpp((const uint32_t *)buffer, tmdsbuf + 2 * pixwidth, pixwidth / 2, red_msb, red_lsb);
					queue_add_blocking_u32(&dvi0.q_tmds_valid, &tmdsbuf);
					break;

				default:
					break;
			}
	}
}

// ***************************************************************************************
//
//									Start the DVI driver
//
// ***************************************************************************************

void DVIStart(void) {
	DVISetMode(DVI_MODE_640_480_8);

	vreg_set_voltage(VREG_VSEL);  													// Set CPU voltage
	sleep_ms(10);  																	// Let it settle for 0.01s
	set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);  								// Set the DVI compatible clock speed
	setup_default_uart();  															// Initialise the UART

	dvi0.timing = &DVI_TIMING;  													// Select timing and pinout
	dvi0.ser_cfg = *DVIGetHDMIConfig();

	dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());	// Initialise DVI
	multicore_launch_core1(dvi_core1_main);  										// Run DVI driver on core #1
}
