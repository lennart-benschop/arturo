// ***************************************************************************************
// ***************************************************************************************
//
//		Name : 		dvi_640x480.c
//		Author :	Paul Robson (paul@robsons.org.uk)
//		Date : 		18th December 2024
//		Reviewed :	No
//		Purpose :	DVI Driver 640x480 / 8 bit colour
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

uint8_t framebuf[3 * PLANE_SIZE_BYTES];

struct dvi_inst dvi0;

// ***************************************************************************************
//
//									The main line renderer
//
// ***************************************************************************************

void __not_in_flash("main") dvi_core1_main() {
	dvi_register_irqs_this_core(&dvi0, DMA_IRQ_0);
	dvi_start(&dvi0);
	while (true) {
		for (uint y = 0; y < FRAME_HEIGHT; ++y) {
			uint32_t *tmdsbuf;
			queue_remove_blocking_u32(&dvi0.q_tmds_free, &tmdsbuf);
			for (uint component = 0; component < 3; ++component) {
				tmds_encode_1bpp(
					(const uint32_t*)&framebuf[y * FRAME_WIDTH / 8 + component * PLANE_SIZE_BYTES],
					tmdsbuf + component * FRAME_WIDTH / DVI_SYMBOLS_PER_WORD,
					FRAME_WIDTH
				);
			}
			queue_add_blocking_u32(&dvi0.q_tmds_valid, &tmdsbuf);
		}
	}
}

// ***************************************************************************************
//
//								Neo6502 / RP2040PC Serialiser
//
// ***************************************************************************************

static const struct dvi_serialiser_cfg pico_neo6502_cfg = {
	.pio = DVI_DEFAULT_PIO_INST,
	.sm_tmds = {0, 1, 2},
	.pins_tmds = {14, 18, 16},
	.pins_clk = 12,
	.invert_diffpairs = true
};

// ***************************************************************************************
//
//									Start the DVI driver
//
// ***************************************************************************************

void DVIStart(void) {
	vreg_set_voltage(VREG_VSEL);  													// Set CPU voltage
	sleep_ms(10);  																	// Let it settle for 0.01s
	set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);  								// Set the DVI compatible clock speed
	setup_default_uart();  															// Initialise the UART

	dvi0.timing = &DVI_TIMING;  													// Select timing and pinout
	dvi0.ser_cfg = pico_neo6502_cfg;

	dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());	// Initialise DVI
	multicore_launch_core1(dvi_core1_main);  										// Run DVI driver on core #1
}
