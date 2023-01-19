// ZX-Evo SDLoad Configuration (c) NedoPC 2023
//
// fetch/display data from internal EABs

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

module video_fetch
(
	input  wire clk,
	input  wire rst_n,

	input  wire pix_stb,

	input  wire i_hsync,
	input  wire i_vsync,
	input  wire i_hpix,
	input  wire i_vpix,

	input  wire v_init,
	input  wire h_init,
	input  wire h_step,
);

	localparam CHAR_ADDR_INIT = 12'h000;
	localparam ATTR_ADDR_INIT = 12'h9C0;
	localparam ATTR_ADDR_ADD  = 12'h028;

	reg [11:0] char_addr;
	reg [11:0] attr_line_addr;





	always @(posedge clk)
	if( pix_stb )
	begin
		char_addr <= CHAR_ADDR_INIT;
	end

	always @(posedge clk)
	if( pix_stb )
	begin
		attr_line_addr <= ATTR_ADDR_INIT;
	end


endmodule

