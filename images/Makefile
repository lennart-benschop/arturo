# ***************************************************************************************
# ***************************************************************************************
#
#		Name : 		Makefile
#		Author :	Paul Robson (paul@robsons.org.uk)
#		Date : 		18th December 2024
#		Reviewed :	No
#		Purpose :	Main firmware makefile, most of the work is done by CMake.
#
# ***************************************************************************************
# ***************************************************************************************

include ../build_env/common.make
include $(BUILDENVDIR)/config.make

PROJECT = kernel

CMAKEOPTS = -DPICO_HARDWARE=$(PICO_HARDWARE)

all: build

always:

# ***************************************************************************************
#
#									Preliminary setup stuff
#
# ***************************************************************************************

prelim:
	mkdir -p $(BINDIR)
	rm -f $(BINDIR)*.elf $(BINDIR)*.uf2
	cd firmware/kernel/locale ; $(PYTHON) keymaps.py >../include/data/__locale.h

# ***************************************************************************************
#
#									Build the firmware
#
# ***************************************************************************************

run: build upload
	
build: prelim always
	cmake --fresh -B build $(CMAKEOPTS)
	$(MAKE) -C build
	cp build/firmware/$(PROJECT)/$(PROJECT).elf $(BINDIR)
	cp build/firmware/$(PROJECT)/$(PROJECT).uf2 $(BINDIR)

# ***************************************************************************************
#
#								Clean build directories
#
# ***************************************************************************************

clean: always
	rm -Rf  build/*
	cmake --fresh -B build $(CMAKEOPTS)
	cmake --build build --target clean

# ***************************************************************************************
#
#								Upload via openocd/
#
# ***************************************************************************************

upload: 
	$(UPLOADER) $(UPCONFIG) $(UPCOMMANDS)