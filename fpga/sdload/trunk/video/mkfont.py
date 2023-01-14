#!/usr/bin/env python3

"""
// ZX-Evo SDLoad Configuration (c) NedoPC 2023
//
// font generator: takes 6912-positioned font and generates font in internal FPGA format

/*
    This file is part of ZX-Evo Base Configuration firmware.

    ZX-Evo Base Configuration firmware is free software:
    you can redistribute it and/or modify it under the terms of
    the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ZX-Evo Base Configuration firmware is distributed in the hope that
    it will be useful, but WITHOUT ANY WARRANTY; without even
    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ZX-Evo Base Configuration firmware.
    If not, see <http://www.gnu.org/licenses/>.
*/
"""

import argparse,os,sys

class ZXPic:

	def __init__(self,filename):
		
		with open(filename,'rb') as file:
			self.zxscr = bytes(file.read())

		if( len(self.zxscr)!=6144 and len(self.zxscr)!=6192 ):
			sys.exit('Wrong zx file <{}> size, must be 6144 or 6912'.format(filename))

		if( len(self.zxscr)==6912 ):
			self.colored = True
		else:
			self.colored = False

		self.pixels = bytes(self.zxscr[:6144])

		if( self.colored ):
			self.attrs = bytes(self.zxscr[6144:])
		else:
			self.attrs = None


	def get_pixel(self,x,y):
		
		if( x<0 or x>255 or y<0 or y>191 ):
			sys.exit('x,y must be within 0..255 and 0..191 range!')


		bitnum = 7 - (x & 7)

		offset = (x>>3) + (y & 7)*256 + ((y & 0x38)>>3)*32 + ((y & 0xC0)>>6)*2048

		return True if self.pixels[offset] & (1<<bitnum) else False







def main():

	# parse arguments
	p = argparse.ArgumentParser()
	#
	p.add_argument('--scr', '-s',           action='store', help='6912 or 6144 ZX screen with font')
	p.add_argument('--out', '-o',           action='store', help='Filename prefix for resulting file(s). Extensions will be added as needed')
	p.add_argument(         '-x', type=int, action='store', help='Initial X position of 8x8 block with first symbol (that must be a space)')
	p.add_argument(         '-y', type=int, action='store', help='Initial Y position of 8x8 block with first symbol (that must be a space)')
	#
	args = p.parse_args()


if __name__=="__main__":
	main()

