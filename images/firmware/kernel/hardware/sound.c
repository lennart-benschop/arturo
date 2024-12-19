// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      sound.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//      Date :      18th August 2024
//      Reviewed :  No
//      Purpose :   PWM Audio support for Neo6502
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "dvi.h"


static int sampleFrequency = -1;
static bool combineSoundChannels = false;

// ***************************************************************************************
//
//      Function that returns the sample rate in Hz of the implementeing hardware
//
// ***************************************************************************************

int SNDGetSampleFrequency(void) {
    if (sampleFrequency < 0) {
        sampleFrequency = DVI_TIMING.bit_clk_khz * 1024 / SAMPLE_DIVIDER / 255;
    }
    return sampleFrequency;
}

// ***************************************************************************************
//
//                                  Interrupt Handler
//
// ***************************************************************************************

void pwm_interrupt_handler() {
    pwm_clear_irq(pwm_gpio_to_slice_num(AUDIO_PIN_L));    
    uint8_t sample0 = SNDGetNextSample(0)+128;
    pwm_set_gpio_level(AUDIO_PIN_L,sample0);
    if (AUDIO_HARDWARE_CHANNELS == 2) {
        uint8_t sample1 = (combineSoundChannels ? sample0 : SNDGetNextSample(1)+128);
        pwm_set_gpio_level(AUDIO_PIN_R,sample1);
    }
}

// ***************************************************************************************
//
//          Initialise a specific channel, only doing the interrupt for the first
//                      (both driven off the same interrupt)
//
// ***************************************************************************************

static void _SND_Initialise_Channel(int pin,bool enableInterrupt) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    int pin_slice = pwm_gpio_to_slice_num(pin);
    if (enableInterrupt) {
        // Setup PWM interrupt to fire when PWM cycle is complete
        pwm_clear_irq(pin_slice);
        // set the handle function above
        irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler); 
        irq_set_enabled(PWM_IRQ_WRAP, true);
        pwm_set_irq_enabled(pin_slice, true);
    }
    // Setup PWM for audio output
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, SAMPLE_DIVIDER); 
    pwm_config_set_wrap(&config, 255); 
    pwm_init(pin_slice, &config, true);
    //
    pwm_set_gpio_level(pin, 0);
}

// ***************************************************************************************
//
//				                  Initialise sound system
//
// ***************************************************************************************

void SNDInitialise(bool _combineChannels) {
    combineSoundChannels = _combineChannels;
    _SND_Initialise_Channel(AUDIO_PIN_L,true);
    if (AUDIO_HARDWARE_CHANNELS == 2) {
        _SND_Initialise_Channel(AUDIO_PIN_R,false);
    }    
}


