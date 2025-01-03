# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		Makefile
#		Purpose :	BDF Font code
#		Date :		1st January 2025
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

include ../../../build_env/common.make

BPYTHON = bdflib/bin/python

convert:
	make -B -C fonts fonts
	$(PYTHON) makeindex.py
	cp fontinclude.h ../include

