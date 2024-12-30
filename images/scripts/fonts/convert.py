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

# ***************************************************************************************
#
#							Represent a single character bitmap
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

	def print(self):
		for d in self.data:
			s = "".join(["*" if (d & (0x8000 >> i) != 0) else "." for i in range(0,16)])
			print(s)
		print()

# ***************************************************************************************
#
#							Represent a single font
#
# ***************************************************************************************

class Font(object):
	def __init__(self,source):
		self.data = [x for x in open(source,"rb").read(-1)]  						# Read in the font
		self.height = self.data[0]  												# CR height
		self.xSpacing = self.data[1]   												# Additional width.
		self.lastChar = self.data[2] 												# End character
		self.chars = {}

		for i in range(32,self.lastChar+1):
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

			newChar.print()

	def getAddr(self,pos):
		return pos + (self.data[pos])+(self.data[pos+1] & 0x3F) * 256


f = Font("fonts/Hemery.fzx")		

