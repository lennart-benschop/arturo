# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		Makefile
#		Purpose :	Build Emulator
#		Date :		22nd December 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

include ../build_env/common.make

APPNAME = artsim

SOURCE1 = $(shell find source -name "*.c")

SOURCES = $(SOURCE1)
OBJECTS = $(subst .c,.o,$(SOURCES))
INCLUDES = -I include -I __include_x

TGTBIN = $(BINDIR)$(APPNAME)
KERNELDIR = $(ROOTDIR)images/firmware/kernel/

CC = gcc
SDL_CFLAGS = $(shell sdl2-config --cflags)
SDL_LDFLAGS = $(shell sdl2-config --libs)
LDFLAGS = 

# ***************************************************************************************
#
#							Build application / compile files
#
# ***************************************************************************************

build : prebuild $(TGTBIN)

run : build
	$(TGTBIN)

$(TGTBIN): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) $(SDL_LDFLAGS) -o $@

%.o:%.c
	$(CC) $(CADDRESSES) $(CFLAGS) $(SDL_CFLAGS) $(INCLUDES) -c -o $@ $<

# ***************************************************************************************
#
#						Clean & Import interface from firmware
#
# ***************************************************************************************

clean:

# ***************************************************************************************
#
#						Prebuild stuff, generating files & tables		
#
# ***************************************************************************************

prebuild:
	rm -f __include_x
	ln -s $(KERNELDIR)include __include_x