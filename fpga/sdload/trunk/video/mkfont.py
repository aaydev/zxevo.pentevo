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

		if( len(self.zxscr)!=6144 and len(self.zxscr)!=6912 ):
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

		self.sz_x = 256
		self.sz_y = 192


	def get_pix(self,x,y):
		
		if( x<0 or x>=self.sz_x or y<0 or y>=self.sz_y ):
			sys.exit('x,y must be within 0..{} and 0..{} range!'.format(self.sz_x-1,self.sz_y-1))


		bitnum = 7 - (x & 7)

		offset = (x>>3) + (y & 7)*256 + ((y & 0x38)>>3)*32 + ((y & 0xC0)>>6)*2048

		return True if self.pixels[offset] & (1<<bitnum) else False


class CharSet:

	def __init__(self, num_els, first_idx, sz_x, sz_y):

		num_els   = int(num_els)
		first_idx = int(first_idx)
		sz_x      = int(sz_x)
		sz_y      = int(sz_y)

		# check arguments
		assert num_els>0, 'num_els must be positive!'
		assert first_idx>=0, 'first_idx must be non-negative!'
		assert sz_x>0, 'sz_x must be positive!'
		assert sz_y>0, 'sz_y must be positive!'

		self.num_els   = num_els
		self.first_idx = first_idx
		self.sz_x      = sz_x
		self.sz_y      = sz_y

		# generate empty characters
		self.charset = [None] * (self.first_idx + self.num_els)

		for char_idx in range(self.first_idx, self.first_idx + self.num_els):
			
			char = [None] * self.sz_y

			for char_y in range(self.sz_y):

				line = [False] * self.sz_x;

				char[char_y] = line

			self.charset[char_idx] = char

	
	def set_pix(self, char_idx, char_y, char_x, value):

		self.charset[char_idx][char_y][char_x] = value


	def get_pix(self, char_idx, char_y, char_x):

		return self.charset[char_idx][char_y][char_x]



def generate_font(pic, start_cx=0, start_cy=0, blk_sx=8, blk_sy=8, box_offx=0, box_offy=0, box_sx=6, box_sy=6, first_idx=32, num_els=224):
# pic -- byte pic with sizes pic.sz_x, pic.sz_y and get_pix(x,y)
# blk_sx/y -- size of font blocks (bounding boxes), typical 8x8
# start_cx/cy -- coord of first element of font, in blocks (typical upper left, 0/0)
# box_offx/y -- offset of actual box with a letter inside block (typical 0/0)
# box_sx/y -- actual box size, 6/6 for 6x6 font etc.
# first_idx -- font index corresponding to start_cx/y position
# num_els -- how many font elements to parse

	# check args
	start_cx = int(start_cx)
	start_cy = int(start_cy)
	blk_sx = int(blk_sx)
	blk_sy = int(blk_sy)
	box_offx = int(box_offx)
	box_offy = int(box_offy)
	box_sx = int(box_sx)
	box_sy = int(box_sy)
	first_idx = int(first_idx)
	num_els = int(num_els)

	assert first_idx>=0
	assert num_els>0

	assert blk_sx>0
	assert blk_sy>0

	assert start_cx>=0
	assert start_cy>=0

	assert blk_sx*(start_cx+1) <= pic.sz_x
	assert blk_sy*(start_cy+1) <= pic.sz_y

	assert box_offx>=0
	assert box_offy>=0

	assert box_sx>0
	assert box_sy>0

	assert box_offx+box_sx <= blk_sx, 'box_offx+box_sx > blk_sx!'
	assert box_offy+box_sy <= blk_sy, 'box_offy+box_sy > blk_sy!'
	

	# create empty font
	font = CharSet(num_els, first_idx, box_sx, box_sy)

	# load font data
	curr_blk_x = start_cx
	curr_blk_y = start_cy

	pic_overflow = False

	for char_idx in range(first_idx, first_idx + num_els):
		
		assert not pic_overflow

		# x/y of upper left part of the box
		x_origin = curr_blk_x*blk_sx + box_offx
		y_origin = curr_blk_y*blk_sy + box_offy

		# copy pixels
		for y in range(box_sy):
			for x in range(box_sx):
				font.set_pix(char_idx, y, x, pic.get_pix(x_origin + x, y_origin + y))
		
		# step to next char in bitmap
		curr_blk_x = curr_blk_x + 1
		if( curr_blk_x*blk_sx >= pic.sz_x ):
			curr_blk_x = 0
			curr_blk_y = curr_blk_y + 1
			if( curr_blk_y*blk_sy >= pic.sz_y ):
				curr_blk_y = 0
				pic_overflow = True


	return font




def gen_binary(font):
	
	binary = bytearray(1024) #zeroed

	for i in range(32,256):
		for y in range(6):
			for x in range(6):		
			
				# actual layout of the resulting font is defined here
				offs = ((i>>3)*36 + x + y*6) & 0x3FF
				#
				bit = 1<<(7-(i&7))

				if font.get_pix(i,y,x):
					binary[offs] = binary[offs] | bit

	return binary



def main():

	# parse arguments
	p = argparse.ArgumentParser()
	#
	p.add_argument('--scr', '-s',           action='store', required=True, help='Filename of 6912 or 6144 ZX screen with font')
	p.add_argument('--out', '-o',           action='store', required=True, help='Filename prefix for resulting file(s). Extensions will be added as needed')
	p.add_argument(         '-x', type=int, action='store', default=0,     help='Initial X position of 8x8 block with first symbol (that must be a space)')
	p.add_argument(         '-y', type=int, action='store', default=0,     help='Initial Y position of 8x8 block with first symbol (that must be a space)')
	#
	args = p.parse_args()


	pic = ZXPic(args.scr)

	font = generate_font(pic=pic,
	                     start_cx=0, start_cy=1,
	                     first_idx=32, num_els=224)

	binary = gen_binary(font)


	bin_name = args.out + ".bin"

	with open(bin_name,"wb") as wrbin:
		wrbin.write(binary)


if __name__=="__main__":
	main()

