// ZX-Evo SDLoad Configuration (c) NedoPC 2023
//
// video sync module

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

module video_sync
(
	input  wire clk,
	input  wire rst_n,

	input  wire vga_on,
	input  wire hsync_polarity, // 1 - positive polarity, 0 - negative
	input  wire vsync_polarity, //

	// pixel strobe -- everything happens enabled by this signal
	output reg  pix_stb,

	// initial sync signals
	output reg  i_hsync,
	output reg  i_vsync,
	output reg  i_hpix,
	output reg  i_vpix,

	// fetch synchronizing signals
	output reg  v_init, // prepare fetching whole screen
	output reg  v_step, // step to the next screen line
	output reg  v_char, // step to the next char line
	
	output reg  h_init, // prepare fetching/displaying single line 
	output reg  h_char  // strobes 6 pix_stb's before the pix_stb that begins h_pix,
	                    // then continues throughout the visible area. Ends also 6 pix_stb's
	                    // before the end of h_pix
);

	parameter H_PERIOD = 9'd448; // in 7MHz clock
	//
	parameter H_TV_SYNC_END  = 9'd33; // counting starts with HSYNC going active
	parameter H_TV_PIX_START = 9'd78;
	parameter H_TV_PIX_STOP  = 9'd438;
	//
	parameter H_VGA_SYNC_END  = 9'd53;
	parameter H_VGA_PIX_START = 9'd79;
	parameter H_VGA_PIX_STOP  = 9'd439;


	parameter V_PERIOD = 9'd262; // in 15625Hz clock
	//
	parameter V_SYNC_END = 9'd2; // in TV mode, must be a little longer than exact 2 HSYNC periods. Addition is 78 7MHz clocks
	parameter V_PIX_START = 9'd18;
	parameter V_PIX_STOP  = 9'd258;


	reg [1:0] pix_divider = 2'b0;


	reg [8:0] h_counter = 9'd0;


	reg v_div2 = 1'b0;

	reg [8:0] v_counter = 9'd0;

	


	// pixel clock strobes
	always @(posedge clk)
		pix_divider[1:0] <= { pix_divider[0], ~pix_divider[1] };
	//
	always @(posedge clk)
		pix_stb <= vga_on ? (~^pix_divider) : (&pix_divider);



	// horizontal counter: counts from 1 to 448
	wire h_count_end = &h_counter[8:6]; // 448 is 0x1C0
	//
	always @(posedge clk)
	if( pix_stb )
	begin
		if( h_count_end )
			h_counter <= 9'd1;
		else
			h_counter <= h_counter + 9'd1;
	end

	// hsync on/off
	wire hsync_on  = h_count_end;
	wire hsync_off = vga_on ? (h_counter==H_VGA_SYNC_END) : (h_counter==H_TV_SYNC_END);

	// hpix on/off
	wire hpix_on  = vga_on ? (h_counter==H_VGA_PIX_START) : (h_counter==H_TV_PIX_START);
	wire hpix_off = vga_on ? (h_counter==H_VGA_PIX_STOP) : (h_counter==H_TV_PIX_STOP);


	// skip every second vertical count in vga mode
	always @(posedge clk)
	if( pix_stb && h_count_end )
		v_div2 <= vga_on ? (~v_div2) : 1'b1;

	// vertical count strobe
	wire v_stb = pix_stb & h_count_end & v_div2;

	// vertical counter: from 1 to 262
	wire v_count_end = v_counter[8] & (&v_counter[2:1]); // 262 is 0x106
	//
	always @(posedge clk)
	if( v_stb )
	begin
		if( v_count_end )
			v_counter <= 9'd1;
		else
			v_counter <= v_counter + 9'd1;
	end

	// vsync on/off
	wire vsync_on  = v_count_end;
	wire vsync_off = (v_counter==V_SYNC_END);

	// vpix on/off
	wire vpix_on  = (v_counter==V_PIX_START);
	wire vpix_off = (v_counter==V_PIX_STOP);


	// make initial sync signals
	always @(posedge clk)
	if( pix_stb )
	begin
		if( hsync_on )
			i_hsync <= hsync_polarity;
		else if( hsync_off )
			i_hsync <= ~hsync_polarity;

		if( hpix_on )
			i_hpix <= 1'b1;
		else if( hpix_off )
			i_hpix <= 1'b0;
	end
	//
	// vsync in tv mode must be 78 pix_stb's longer
	reg [7:0] extra_vsync_count;
	reg       extra_vsync_count_r;
	always @(posedge clk)
	if( v_stb && vsync_off )
			extra_vsync_count <= 8'd77 + 8'h80;
	else if( pix_stb && extra_vsync_count[7] )
			extra_vsync_count <= extra_vsync_count - 8'd1;
	//
	always @(posedge clk)
	if( pix_stb )
		extra_vsync_count_r <= extra_vsync_count[7];
	//
	always @(posedge clk)
	begin
		if( v_stb && vsync_on )
			i_vsync <= vsync_polarity;
		else if( vga_on ? (v_stb && vsync_off) : (pix_stb && extra_vsync_count_r && !extra_vsync_count[7]) )
			i_vsync <= ~vsync_polarity;
	end
	//
	always @(posedge clk)
	if( v_stb )
	begin
		if( vpix_on )
			i_vpix <= 1'b1;
		else if( vpix_off )
			i_vpix <= 1'b0;
	end



	// vertical fetch syncs

	always @(posedge clk)
	if( pix_stb )
	begin
		if( v_stb && vsync_off )
			v_init <= 1'b1;
		else
			v_init <= 1'b0;
	end


	reg [2:0] vctr_6;
	always @(posedge clk)
	if( pix_stb )
	begin
		if( ?? )
			vctr_6
		else if( v_stb )
			vctr_6 <= (vctr_6[2] & vctr_6[0]) ? 3'd0 : (vctr_6 + 3'd1);
	end


	always @(posedge clk)
	if( pix_stb )
	begin
		if( i_vpix && v_stb )
			v_step <= 1'b1;
		else
			v_step <= 1'b0;
	end
	



	// horizontal fetch syncs

	always @(posedge clk)
	if( pix_stb )
	begin
		if( hsync_off )
			h_init <= 1'b1;
		else
			h_init <= 1'b0;
	end

	
	wire start_char = vga_on ? (h_counter==(H_VGA_PIX_START-7)) : (h_counter==(H_TV_PIX_START-7));
	wire stop_char  = vga_on ? (h_counter==(H_VGA_PIX_STOP -7)) : (h_counter==(H_TV_PIX_STOP -7));

	reg char;
	always @(posedge clk)
	if( pix_stb )
		if( start_char )
			char <= 1'b1;
		else if( stop_char )
			char <= 1'b0;

	// MOD 6 counter
	reg [2:0] char_ctr;
	//
	always @(posedge clk)
	if( pix_stb )
	begin
		if( start_char )
			char_ctr <= 3'd0;
		else
			char_ctr <= (char_ctr[2] & char_ctr[0]) ? 3'd0 : (char_ctr + 3'd1);
	end
	//

	//
	always @(posedge clk)
	if( pix_stb )
	begin
		if( start_char )
			h_char <= 1'b1;
		else if( char && (char_ctr[2] & char_ctr[0]) )
			h_char <= 1'b1;
		else
			h_char <= 1'b0;
	end




endmodule

