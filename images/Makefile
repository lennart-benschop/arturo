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

CONFIG_OUT_FILE = firmware/kernel/include/data/__config.h
LOCALE_OUT_FILE = ../include/support/__locale.h

prelim: $(CONFIG_OUT_FILE)
	mkdir -p $(BINDIR)
	rm -f $(BINDIR)*.elf $(BINDIR)*.uf2
	cd firmware/kernel/locale ; $(PYTHON) keymaps.py >$(LOCALE_OUT_FILE)

$(CONFIG_OUT_FILE) : $(BUILDENVDIR)/config.make
	$(PYTHON) scripts/makeconfig.py '$(CONFIG_DATA)' >$(CONFIG_OUT_FILE)
	
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

	