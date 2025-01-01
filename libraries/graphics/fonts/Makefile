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
	$(BPYTHON) convert.py 
	cp fontdata.h ..
	cp fontinclude.h ../include

create-venv:
	rm -Rf bdflib
	$(PYTHON) -m venv bdflib
	bdflib/bin/pip install bdflib

