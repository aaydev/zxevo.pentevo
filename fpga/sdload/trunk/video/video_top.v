// ZX-Evo SDLoad Configuration (c) NedoPC 2023
//
// top module for video output.

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

module video_top
(
	input  wire fclk,
	input  wire rst_n,

	// config inputs
	input  wire vga_on,

	// data write iface
	/* TODO */

	// video output
	output wire vsync,
	output wire hsync,
	output wire csync,
	output wire [1:0] red,
	output wire [1:0] grn,
	output wire [1:0] blu
);


	wire pix_stb;
	
	wire i_hsync, i_vsync,
	     i_hpix,  i_vpix;
	
	wire v_init, h_init,
             h_step, h_char;




	video_sync video_sync
	(
		.clk  (fclk ),
		.rst_n(rst_n),

		.vga_on        ( vga_on),
		.hsync_polarity(~vga_on),
		.vsync_polarity(~vga_on),

		.pix_stb(pix_stb),

		.i_hsync(i_hsync),
		.i_vsync(i_vsync),
		.i_hpix (i_hpix ),
		.i_vpix (i_vpix ),

		.v_init(v_init),
		.h_init(h_init),
		.h_step(h_step),
		.h_char(h_char)
	);





endmodule

