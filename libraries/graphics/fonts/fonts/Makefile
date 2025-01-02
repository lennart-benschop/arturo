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


SRCFONTS = $(shell find . -name "*.ttf")

fonts: clean $(SRCFONTS)

clean:
	rm -f *.bdf
	
%.ttf:
	otf2bdf $@ -p 8 -o  $(*F)08.bdf || true
	otf2bdf $@ -p 10 -o $(*F)10.bdf || true
	otf2bdf $@ -p 12 -o $(*F)12.bdf || true
	otf2bdf $@ -p 14 -o $(*F)14.bdf || true
	otf2bdf $@ -p 18 -o $(*F)18.bdf || true

