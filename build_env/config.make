# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		config.make
#		Purpose :	Configuration files
#		Date :		18th December 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

# *******************************************************************************************
#
#										Configuration
#
# *******************************************************************************************
#
#		Set these accordingly. 
#
export PICO_SDK_PATH := /aux/riscv/pico-sdk/
export PICO_DVI_PATH := /aux/riscv/PicoDVI/
#
#		Configure Platform (rp2040,rp2350,rp2350-riscv)
#
PLATFORM = rp2040
#
#		Specifies the physical hardware used (PICO_HW_RP2040PC,PICO_HW_NEO6502)
#
PICO_HARDWARE = PICO_HW_RP2040PC
#
# 		Specifies the function to execute to run the application once everything is initialised
#
ARTURO_RUN_FUNCTION = TESTApplication
#
# 		If non-zero, USB keyboard messages are converted to ASCII and Localised. It also allows
# 		the state of each key (up or down) to be read. If zero, then the responsibility for 
#		handling USB Key events lies with the author.
#
ARTURO_PROCESS_KEYS = 1
#
#		If non-zero, use mono sound (e.g. combine the two RP2040PC channels into one)
#
ARTURO_MONO_SOUND = 0

# *******************************************************************************************
#
#									  OpenOCD Configuration
#
# *******************************************************************************************
#
#		OpenOCD must be build as per "Getting started with Raspberry PI Pico" Appendix A
#		Most distro builds currently won't work.
#
UPLOADER = openocd 
UPCONFIG = -f interface/cmsis-dap.cfg -f target/$(PLATFORM).cfg 
UPCOMMANDS = -c "adapter speed 5000" -c "program $(BINDIR)$(PROJECT).elf verify reset exit"

# *******************************************************************************************
#
#									  Serial Configuration
#
# *******************************************************************************************

PICO_SERIAL_PORT = /dev/ttyUSB0
PICO_SERIAL_BAUD_RATE = 115200

# *******************************************************************************************
#
#					Used to output the data to __config.h. 
#
# *******************************************************************************************

#
#		DO NOT MANUALLY CHANGE THESE !
#
CONFIG_DATA="\
\#define PICO_PLATFORM_NAME "$(PLATFORM)" |\
\
\#define PICO_HW_RP2040PC 	(0) |\
\#define PICO_HW_NEO6502 	(1) |\
\#define PICO_HARDWARE 		$(PICO_HARDWARE) 	|\
\
\#define ARTURO_RUN_FUNCTION $(ARTURO_RUN_FUNCTION) |\
\#define ARTURO_PROCESS_KEYS $(ARTURO_PROCESS_KEYS) |\
\#define ARTURO_MONO_SOUND   $(ARTURO_MONO_SOUND) |\
"
