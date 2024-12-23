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

SOURCES = source/main.o

INCLUDES = -I include

TGTBIN = $(BINDIR)$(APPNAME)

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

$(TGTBIN): $(SOURCES)
	$(CC) $(SOURCES) $(LDFLAGS) $(SDL_LDFLAGS) -o $@

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
