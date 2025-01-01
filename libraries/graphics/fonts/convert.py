# *******************************************************************************************
# *******************************************************************************************
#
#		Name : 		convert.py
#		Purpose :	Convert BDF file to internal format.
#		Date :		1st January 2025
#		Author : 	Paul Robson (paul@robsons.org.uk)
#
# *******************************************************************************************
# *******************************************************************************************

import os,sys

# *******************************************************************************************
#
#										Represents a font
#
# *******************************************************************************************

from bdflib import reader,xlfd

class Font(object):
	#
	#		Create the font object.
	#
	def __init__(self,fontFile):
		with open(fontFile,"rb") as h:  														# Read in the font.
			self.font = reader.read_bdf(h)
		xlfd.validate(self.font)   																# validate the font definition string.
		self.height = self.font[xlfd.PIXEL_SIZE]
		self.baseLine = self.font[xlfd.FONT_ASCENT]  											# Top to baseline.
		self.totalHeight = self.font[xlfd.FONT_DESCENT]+self.baseLine   						# Complete range
	#
	#		Get Information
	#
	def getSize(self):
		return self.font.ptSize
	#
	#		Get glyph
	#
	def getGlyph(self,n):
		return self.font[n] if n in self.font else None
	#
	#		Get glyph repreesentation
	#	
	def getGlyphRepresentation(self,n):
		d = [x for x in self.getGlyph(n).data]
		d.reverse()
		if sum(d) != 0:  																		# If there are any non zero values
			while len([x for x in d if (x & 0x80000000) != 0]) == 0:
				d = [x << 1 for x in d]
		return d
	#
	#		Get header size (blank lines above)
	#
	def getHeaderSize(self,n):
		g = self.getGlyph(n)  																	# Glyph
		topLine = g.bbY + g.bbH 	  															# Top line of character, using baseline as Y = 0		
		return self.baseLine - topLine
	#
	#		Get footer size (blank lines below)
	#
	def getFooterSize(self,n):
		g = self.getGlyph(n)  																	# Glyph
		h = self.getHeaderSize(n)  																# How much above
		return self.totalHeight - h - len(g.data)
	#
	#		Print character
	#
	def print(self,n):
		d = self.getGlyphRepresentation(n)
		print(self.getGlyph(n).get_bounding_box())
		print("{2} Height:{0} Header:{1} Footer:{3} Full:{4}\n".format(self.height,self.getHeaderSize(n),n,self.getFooterSize(n),self.totalHeight))
		print("\n".join(["{0:32b}".format(n).replace(" ",".").replace("0",".").replace("1","*") for n in d]))
	#
	#		Get the known name from the family, style, bold/italic and size, and the associated identifier.
	#
	def getName(self):
		s = str(self.font[xlfd.FAMILY_NAME])[2:]
		d = str(self.font[xlfd.ADD_STYLE_NAME])
		s = s if d == "" else s+d[2:]
		d = "Bold" if self.font[xlfd.WEIGHT_NAME].lower() == "bold" else ""
		d += ("Italic" if self.font[xlfd.SLANT] == xlfd.SLANT_ITALIC else "")
		s = s if d == "" else s + "_"+d
		return s.replace(" ","").replace("'","").lower()+"_"+str(self.getSize())
	#
	def getIdentifier(self):
		return "_"+self.getName()+"_data"
	#
	#		Render the font as data.
	#
	def render(self):
		self.data = [ 32, 127,self.totalHeight,self.baseLine]   								# +0 First char, +1 Last char, +2 total overall height, +3 baseline from top
		self.data += [ 0 ] * (16 - len(self.data))  											# pad out to 16 with 0 bytes
		for c in self.getName():   																# Get the name of the font and add that
			self.data.append(ord(c))
		self.data += [ 0 ] * (64 - len(self.data))  											# pad out to 64 with 0 bytes
		assert len(self.data) == 64
		for i in range(self.data[0],self.data[1]+1):   											# add empty records for offsets to character descriptions.			
			self.data += [0xFF,0xFF]
		for c in range(self.data[0],self.data[1]+1):  											# for each character.
			if self.getGlyph(c) is not None:
				pos = (c - self.data[0]) * 2 + 64   											# table entry offset
				self.data[pos] = len(self.data) & 0xFF   										# Put an offset to the actual data.
				self.data[pos+1] = len(self.data) >> 8
				self.renderGlyph(c)  															# Render the glyph
		return self.data
	#
	#		Render a single glyph
	#
	def renderGlyph(self,c):
		pos = len(self.data)
		width = self.getWidth(c)   																# Get the pixel width
		width = width if width != 0 else self.getWidth(ord('!'))			 					# for space use the width of the pling.
		data = self.getGlyphRepresentation(c)   												# data size
		self.data += [ width, self.getHeaderSize(c),len(data) ] 								# +0 width in pixels, +1 leading blank lines,+2 lines of pixels

		self.data.append(0)  																	# set up, first bit to do is the msb of the last byte
		bitMask = 0x80
		for d in data:   																		# for each data line
			for x in range(0,width):  															# check each bit from the MSB
				if d & (0x80000000 >> x) != 0:  												# Pixel set.
					self.data[-1] |= bitMask   													# Set output bit
				bitMask >>= 1  																	# Next bit/byte
				if bitMask == 0:
					bitMask = 0x80
					self.data.append(0)
	#
	#		Calculate character width using bit data
	#
	def getWidth(self,c):
		data = self.getGlyphRepresentation(c)  													# Get data
		for size in range(0,36):  																# See how far we shift left to remove all of it.
			if sum([(n << size) & 0xFFFFFFFF for n in data]) == 0:
				return size
		assert False


fontList = []
h = open("fontdata.h","w")
h.write("//\n//\tThis file is automatically generated.\n//\n")
h.write("#pragma once\n\n")
total = 0

for root,dirs,files in os.walk("fonts"):  														# Scan the directory tree for BDF files
	for f in files:
		if f.endswith(".bdf"):
			#print(">>>>",f)
			font = Font(root+os.sep+f)  														# create it
			data = font.render()  																# convert it
			h.write("// Font {0} size {1} bytes\n\n".format(font.getName(),len(data)))			# output it
			h.write("const uint8_t {0}[] = {{ {1} }};\n\n".format(font.getIdentifier(),",".join([str(x) for x in data])))
			fontList.append(font)
			total += len(data)

fontList.sort(key = lambda x:x.getName())
h.write("const uint8_t *fontTable[] = {{ {0} }};\n\n".format(",".join([f.getIdentifier() for f in fontList])))
h.write("// total size {0} bytes\n\n".format(total))
h.close()

h = open("fontinclude.h","w")
h.write("//\n//\tThis file is automatically generated.\n//\n")
h.write("#pragma once\n\n")

h.write("#define FONT_COUNT ({0})\n".format(len(fontList)))
for i in range(0,len(fontList)):
	h.write("#define FONT_{0} ({1})\n".format(fontList[i].getName().upper(),i))
h.close()