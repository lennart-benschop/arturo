#ifndef _TMDS_ENCODE_CUSTOM_H_
#define _TMDS_ENCODE_H_

#include "hardware/interp.h"
#include "dvi_config_defs.h"

// Functions from tmds_encode.c

void tmds_encode_custom_1bpp(const uint32_t *pixbuf, uint32_t *symbuf, size_t n_pix);
void tmds_encode_custom_2bpp(const uint32_t *pixbuf, uint32_t *symbuf, size_t n_pix);


#endif
