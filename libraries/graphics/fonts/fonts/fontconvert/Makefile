all: fontconvert

CC     = gcc
CFLAGS = -Wall -I/usr/local/include/freetype2 -I/usr/include/freetype2 -I/usr/include
LIBS   = -lfreetype

fontconvert: fontconvert.c
	$(CC) $(CFLAGS) -D COMPILEME fontconvert.c $(LIBS) -o $@
	strip $@

clean:
	rm -f fontconvert
