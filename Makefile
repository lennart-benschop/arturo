# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		Makefile
#		Purpose :	Build Emulator
#		Date :		29th December 2024
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

include build_env/common.make
include build_env/config.make


# ***************************************************************************************
#
#								Run on hardware or software
#
# ***************************************************************************************

hw:
	make -C images run

sw:
	make -C simulator run

fonts:
	make -B -C libraries/graphics/fonts convert
	rm -f applications/coregfx/graphics.o
	rm -f libraries/graphics/fonts.o