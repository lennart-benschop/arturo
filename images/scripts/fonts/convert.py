# ***************************************************************************************
# ***************************************************************************************
#
#      Name :      convert.py
#      Authors :   Paul Robson (paul@robsons.org.uk)
#      Date :      30th December 2024
#      Reviewed :  No
#      Purpose :   Convert FZX to new format.
#
# ***************************************************************************************
# ***************************************************************************************

import re,os,sys

# ***************************************************************************************
#
#							Represent a single character bitmap
#
# ***************************************************************************************
#
#		Bitmap character Data
#		=====================
#
#		Rendering Width (in pixels, 8 means double bytes of data)
#		Kerning (MSN) Extra Spacing (LSN)
#		Baseline offset from top.
#		# Blank Lines above character
# 		# Lines of character data
#		# Blank Lines below data
#		Binary Data (multiple)
#
# ***************************************************************************************
#
#		Font Data
#		=========
#
#		Height (the recorded height - 1)
#		First character
#		Last character
#		(Pad to 8 bytes)
#		Offset from start of font data to first character (or $FFFF if character not defined)
#
#		Header size is 16 + (number of characters) * 2
#
# ***************************************************************************************

class FontCharacter(object):
	def __init__(self,width,kerning,vertSkip):
		self.width = width  														# Save parameters
		self.kerning = kerning
		self.bytes = 2 if width > 8 else 1
		self.data = [ 0 ] * vertSkip 					  							# Add the initial vertical skip (data is WORDs)

	def pad(self,height):
		while len(self.data) < height:
			self.data.append(0)

	def addRows(self,data):
		while len(data) != 0:
			self.data.append(data.pop(0) << 8) 
			if self.bytes == 2:
				self.data[-1] = self.data[-1] | data.pop(0)

	def getExtent(self):
		self.startData = 0
		self.endData = len(self.data)-1
		while self.startData < len(self.data) and self.data[self.startData] == 0:
			self.startData += 1
		while self.endData >= 0 and self.data[self.endData] == 0:
			self.endData -= 1
		self.startData = min(self.startData,len(self.data))
		self.endData = max(self.startData,self.endData)

	def render(self,baseLineOffset,extraSpace):
		self.renderData =  [ self.width,self.kerning * 16 + extraSpace,baseLineOffset,self.startData,self.endData-self.startData+1,len(self.data)-self.endData-1]

		self.renderData.append(0)   												# Set up first byte, unused
		bitMask = 0x80

		#print(self.startData,self.endData,len(self.data))
		for y in range(self.startData,self.endData+1):   							# Scan vertically, then horizontally
			for x in range(0,self.width): 
				if y < len(self.data):
					if (self.data[y] & (0x8000 >> x)) != 0:   						# Pixel set, set that bit
						self.renderData[-1] |= bitMask

				bitMask >>= 1   														# Shift to next bit
				if bitMask == 0:
					bitMask = 0x80
					self.renderData.append(0)

		#print(self.renderData)
		#self.print()

	def get(self):
		return self.renderData

	def print(self):
		n = 0
		for d in self.data:
			s = "".join(["*" if (d & (0x8000 >> i) != 0) else "." for i in range(0,16)])
			print(n,s)
			n += 1
		print()

# ***************************************************************************************
#
#							Represent a single font
#
# ***************************************************************************************

class Font(object):
	def __init__(self,source):
		self.data = [x for x in open(source,"rb").read(-1)]  						# Read in the font
		self.name = source.split(os.sep)[-1].lower().replace(".fzx","")  			# Store processed name
		self.height = self.data[0]  												# CR height
		self.xSpacing = self.data[1]   												# Additional width.
		self.firstChar = 32
		self.lastChar = self.data[2] 												# End character
		self.chars = {}

		for i in range(self.firstChar,self.lastChar+1):
			charEntry = 3 + (i-32) * 3  											# Offset todata

			kerning = self.data[charEntry+1] >> 6  									# Kerning 
			width = ((self.data[charEntry+2]) & 0xF)+1  							# Character width
			vertSkip = self.data[charEntry+2] >> 4  								# Vertical skip.

			newChar = FontCharacter(width,kerning,vertSkip)  						# Create new character, add to storage.
			self.chars[i] = newChar

			data = self.getAddr(charEntry)  										# Work out font data range
			dataNext = self.getAddr(charEntry+3)
			newChar.addRows(self.data[data:dataNext])  								# Add that data.
			newChar.pad(self.height)  												# Pad out new character with filler
			newChar.getExtent()

	def render(self):
		ct = 'o' if 'o' in self.chars else 'O'
		self.baseLine = self.chars[ord(ct)].endData+1  								# This is the base line for the font.

		for c in self.chars.keys():   												# Convert data to pixels
			self.chars[c].render(self.baseLine,self.xSpacing)
			#self.chars[c].print()

		count = self.lastChar - self.firstChar + 1   								# No of records.
		self.data = [ self.height-1, self.firstChar,self.lastChar,0,0,0,0,0] 		# Header
		self.data += [ 0xFF ] * (count * 2)   										# Blank records 

		for c in range(self.firstChar,self.lastChar+1):  							# For each character
			aPos = 8 + (c - self.firstChar) * 2   									# Update the offset table.
			self.data[aPos] = len(self.data) & 0xFF
			self.data[aPos+1] = len(self.data) >> 8
			self.data += self.chars[c].get()    									# Append created data.
		return self

	def getAddr(self,pos):
		return pos + (self.data[pos])+(self.data[pos+1] & 0x3F) * 256

	def getFontName(self):
		return self.name 

	def getFontIdentifier(self):
		return "_"+self.getFontName()+"_data";

# ***************************************************************************************
#
#							Represent a font collection
#
# ***************************************************************************************

class FontCollection(object):
	def __init__(self,directory):
		self.fonts = {}
		for root,dirs,files in os.walk(directory):
			for f in files:
				if f.endswith(".fzx"):
					nc = Font(root+os.sep+f)
					self.fonts[nc.getFontName()] = nc

	def render(self):
		h = open("__fontdata.h","w")
		h.write("#pragma once\n\n")
		keys = [x for x in self.fonts.keys()]
		keys.sort()
		size = 0
		for f in keys:
			font = self.fonts[f]
			font.render()
			h.write("// {0} bytes\n\n".format(len(font.data)))
			h.write("static const uint8_t {0}[] = {{ {1} }};\n\n".format(font.getFontIdentifier(),",".join([str(x) for x in font.data])))
			size += len(font.data)

		h.write("// Size {0}\n\n".format(size))
		s = ",".join([self.fonts[f].getFontIdentifier() for f in keys])
		h.write("static const uint8_t *_font_list[] = {{ {0} }};\n\n".format(s))
		h.close()

		h = open("__fontinclude.h","w")
		h.write("#pragma once\n\n")
		h.write("#define FONT_COUNT ({0})\n\n".format(len(keys)))
		for i in range(0,len(keys)):
			h.write("#define FONT_{0} ({1})\n".format(self.fonts[keys[i]].getFontName().upper(),i))
		h.close()
		
FontCollection("fonts").render()

