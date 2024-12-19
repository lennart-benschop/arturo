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
#		Configure Platform (rp2350,rp2350-riscv)
#
PLATFORM = rp2040
#
#		Can be SDCARD or USB currently. If you change this use make switch to
#		rebuild as it cmake clean doesn't clean things up enough (probably me)
#
STORAGE = USB
#
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
#
#		Target for Boot-Upload approach
#
UF2UPLOAD = /media/$(USER)/RP2350

# *******************************************************************************************
#
#									  Serial Configuration
#
# *******************************************************************************************

PICO_SERIAL_PORT = /dev/ttyUSB0
PICO_SERIAL_BAUD_RATE = 115200
