all: fontconvert fonts

CC     = gcc
CFLAGS = -Wall -I/usr/local/include/freetype2 -I/usr/include/freetype2 -I/usr/include
LIBS   = -lfreetype

SOURCES = $(wildcard fonts/*.ttf)
OBJECTS = $(patsubst fonts/%.ttf, fonts/%.h, $(SOURCES))

fontconvert: fontconvert.c
	$(CC) $(CFLAGS) $< $(LIBS) -o $@
	strip $@

clean:
	rm -f fontconvert


fonts: $(OBJECTS)

%.h : %.ttf
	rm -f $@
	./fontconvert $< 7 32 127 >>$@
	./fontconvert $< 9 32 127 >>$@
	./fontconvert $< 12 32 127 >>$@
	./fontconvert $< 14 32 127 >>$@
	cp fonts/*.h ../../libraries/graphics/fonts