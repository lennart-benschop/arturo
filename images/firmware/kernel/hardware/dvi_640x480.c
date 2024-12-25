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

struct dvi_serialiser_cfg *DVIGetHDMIConfig(void);

// ***************************************************************************************
//
//				Frame buffer, these are 3 planar bitmaps for 640x480
//
// ***************************************************************************************

#define FRAME_WIDTH 640  															// Not the *pixels*, it's the display setting.
#define FRAME_HEIGHT 480

#define PLANE_SIZE_BYTES (FRAME_WIDTH * FRAME_HEIGHT / 2 / 8) 						// We don't support 640x480x8

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

bool DVISetMode(int mode) {
	bool supported = true;

	dvi_modeInfo.mode = mode;  														// Record mode

	switch(mode) {

		case DVI_MODE_640_240_8: 													// 640x240x8 information.			
			dvi_modeInfo.width = 640;dvi_modeInfo.height = 240;
			dvi_modeInfo.bitPlaneCount = 3;
			dvi_modeInfo.bitPlaneSize = PLANE_SIZE_BYTES;
			dvi_modeInfo.bitPlaneDepth = 1;
			for (int i = 0;i <dvi_modeInfo.bitPlaneCount;i++)
				dvi_modeInfo.bitPlane[i] = framebuf + PLANE_SIZE_BYTES * i;
			dvi_modeInfo.bytesPerLine = dvi_modeInfo.width / 8;  					// Calculate bytes per line.
			break;

		case DVI_MODE_640_480_2: 													// 640x480x2 information.			
			dvi_modeInfo.width = 640;dvi_modeInfo.height = 480;
			dvi_modeInfo.bitPlaneCount = 1;
			dvi_modeInfo.bitPlaneSize = PLANE_SIZE_BYTES;
			dvi_modeInfo.bitPlaneDepth = 1;
			dvi_modeInfo.bitPlane[0] = framebuf;
			dvi_modeInfo.bytesPerLine = dvi_modeInfo.width / 8;  					// Calculate bytes per line.
			break;

		case DVI_MODE_320_240_8:  													// 320x240x8 information.
			dvi_modeInfo.width = 320;dvi_modeInfo.height = 240;
			dvi_modeInfo.bitPlaneCount = 3;
			dvi_modeInfo.bitPlaneSize = PLANE_SIZE_BYTES;
			dvi_modeInfo.bitPlaneDepth = 1;
			for (int i = 0;i <dvi_modeInfo.bitPlaneCount;i++)
				dvi_modeInfo.bitPlane[i] = framebuf + PLANE_SIZE_BYTES * i;
			dvi_modeInfo.bytesPerLine = dvi_modeInfo.width / 8;  					// Calculate bytes per line.
			break;

		default:
			supported = false;
			dvi_modeInfo.mode = -1;  												// Failed.
	}
	return supported;
}

// ***************************************************************************************
//
//									The main line renderer
//
// ***************************************************************************************

static uint8_t _buffer[640];
static uint16_t _mapping[256];

void __not_in_flash("main") dvi_core1_main() {

	uint32_t *tmdsbuf;
	dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
	dvi_start(&dvi0);
	uint y = -1;

	//
	//		This table maps an 8 bit bit pattern into a 'double width' 16 bit pattern.
	//
	for (int i = 0;i < 256;i++) {
		_mapping[i] = 0;
		for (int b = 0;b < 8;b++) {
			if (i & (1 << b)) _mapping[i] |= (3 << (b*2));
		}
		_mapping[i] = (_mapping[i] >> 8) | (_mapping[i] << 8);
	}

	while (true) {
			y = (y + 1) % FRAME_HEIGHT;
			switch(dvi_modeInfo.mode) {
				//
				//		Mode 0 is 640x240x8 colours as 3 bitplanes.
				//
				case DVI_MODE_640_240_8:
					queue_remove_blocking_u32(&dvi0.q_tmds_free, &tmdsbuf);
					for (uint component = 0; component < 3; ++component) {
						tmds_encode_1bpp(
							(const uint32_t*)(framebuf+(y/2)*640/8 + component * PLANE_SIZE_BYTES),
							tmdsbuf + (2-component) * FRAME_WIDTH / DVI_SYMBOLS_PER_WORD,  	// The (2-x) here makes it BGR Acordn standard
							FRAME_WIDTH
						);
					}
					queue_add_blocking_u32(&dvi0.q_tmds_valid, &tmdsbuf);
					break;

				//
				//		Mode 1 is 320x240x8 colours as 3 bitplanes.
				//
				case DVI_MODE_320_240_8:
					queue_remove_blocking_u32(&dvi0.q_tmds_free, &tmdsbuf);
					for (uint component = 0; component < 3; ++component) {
						uint8_t *_source = framebuf+(y/2)*320/8 + component * PLANE_SIZE_BYTES;
						uint16_t *_target = (uint16_t *)_buffer;

						for (int i = 0;i < 320/8;i++) {
							*_target++ = _mapping[*_source++];
						}

						tmds_encode_1bpp(
							(const uint32_t*)_buffer,
							tmdsbuf + (2-component) * FRAME_WIDTH / DVI_SYMBOLS_PER_WORD,  	// The (2-x) here makes it BGR Acordn standard
							FRAME_WIDTH
						);
					}
					queue_add_blocking_u32(&dvi0.q_tmds_valid, &tmdsbuf);
					break;

				//
				//		Mode 2 is 640x480x1 colour as 1 bitplanes.
				//
				case DVI_MODE_640_480_2:
					queue_remove_blocking_u32(&dvi0.q_tmds_free, &tmdsbuf);
					for (uint component = 0; component < 3; ++component) {
						tmds_encode_1bpp(
							(const uint32_t*)(framebuf+y*640/8),
							tmdsbuf + (2-component) * FRAME_WIDTH / DVI_SYMBOLS_PER_WORD,  	// The (2-x) here makes it BGR Acordn standard
							FRAME_WIDTH
						);
					}
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
	DVISetMode(DVI_MODE_640_240_8);
	//DVISetMode(DVI_MODE_320_240_8);
	//DVISetMode(DVI_MODE_640_480_1);

	vreg_set_voltage(VREG_VSEL);  													// Set CPU voltage
	sleep_ms(10);  																	// Let it settle for 0.01s
	set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);  								// Set the DVI compatible clock speed
	setup_default_uart();  															// Initialise the UART

	dvi0.timing = &DVI_TIMING;  													// Select timing and pinout
	dvi0.ser_cfg = *DVIGetHDMIConfig();

	dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());	// Initialise DVI
	multicore_launch_core1(dvi_core1_main);  										// Run DVI driver on core #1
}

