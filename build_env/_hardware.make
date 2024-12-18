# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		hardware.make
#		Purpose :	Common make code for building / uploading RISC images
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
#export PICO_SDK_PATH := /aux/riscv/pico-sdk/
#export PICO_TOOLCHAIN_PATH := /aux/riscv/corev-openhw-gcc-ubuntu2204-20240530/
#export PICO_RISCV_TOOLCHAIN_PATH := $(PICO_TOOLCHAIN_PATH)
#
#		Configure Platform (rp2350,rp2350-riscv)
#
PLATFORM = rp2040
#
#		Can be SDCARD or USB currently. If you change this use make switch to
#		rebuild as it cmake clean doesn't clean things up enough (probably me)
#
STORAGE = USB

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
#
#		Copy the binary to the device, this is for standard UF2 booting, e.g. not using the debugger
#
copyfs:
	cp $(BINDIR)$(PROJECT).uf2 $(UF2UPLOAD)
#
#		Build and upload and run
#
run: build
	$(UPLOADER) $(UPCONFIG) $(UPCOMMANDS)
#
#		Reset Pico
#
reset:
	$(UPLOADER) $(UPCONFIG) -c "adapter speed 5000" -c "init" -c "reset init" -c "reset run" -c "exit"

# *******************************************************************************************
#
#									  Serial Configuration
#
# *******************************************************************************************

PICO_SERIAL_PORT = /dev/ttyUSB0
PICO_SERIAL_BAUD_RATE = 115200
#
#		Start a serial TTY monitor.
#
serial:
	echo "Ctrl-A X to exit Picocom"
	picocom -q -b $(PICO_SERIAL_BAUD_RATE) $(PICO_SERIAL_PORT)
	
