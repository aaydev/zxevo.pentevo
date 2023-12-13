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
	input  wire h_char,



	// char/attr memory read
	output wire        char_r_rdena, // marks valid char_r_addr
	output wire [11:0] char_r_addr,
	input  wire [ 7:0] char_r_data, // 1 cycle latency

	// font memory read
	output wire [ 9:0] font_r_addr,
	input  wire [ 7:0] font_r_data, // 1 cycle latency

);

	localparam CHAR_ADDR_INIT = 12'h000;
	localparam CHAR_LINE_ADD  = 12'd60;

	localparam ATTR_ADDR_INIT = 12'h9C0;
	localparam ATTR_LINE_ADD  = 12'd40;

	reg [11:0] char_line_addr;
	reg [11:0] attr_line_addr;

	reg [11:0] char_addr;

	reg [11:0] attr_addr;
	reg [2:0] attr_phase;




	always @(posedge clk)
	if( pix_stb )
	begin
		if( v_init )
			char_line_addr <= CHAR_ADDR_INIT;
		else if( h_step )
			char_line_addr <= char_line_addr + CHAR_LINE_ADD;
	end
	//
	always @(posedge clk)
	if( pix_stb )
	begin
		if( h_init )
			char_addr <= char_line_addr;
		else if( h_char )
			char_addr <= char_addr + 12'd1;
	end


	always @(posedge clk)
	if( pix_stb )
	begin
		if( v_init )
			attr_line_addr <= ATTR_ADDR_INIT;
		else if( h_step )
			attr_line_addr <= attr_line_addr + ATTR_LINE_ADD;
	end




endmodule

