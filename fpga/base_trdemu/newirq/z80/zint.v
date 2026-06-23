// ZX-Evo Base Configuration (c) NedoPC 2008,2009,2010,2011,2012,2013,2014
//
// frame INT generation

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

`include "../include/tune.v"

module zint
(
	input  wire fclk,
	input  wire rst_n,

	input  wire zpos,
	input  wire zneg,

	// irq initiators
	input  wire vbl_start, // vblank interrupt
	input  wire tmr_start, // timer interrupt
	// TODO: more inputs

	input  wire iorq_n,
	input  wire m1_n,

	input  wire wait_n,

	output reg  int_n,

	input  wire        irq_enh,
	input  wire        irq_ena_int_vec,
	input  wire        irq_ena_ext_vec,
	input  wire        irq_int_autoclr,
	//
	output reg  [ 6:0] irq_stat,
	//
	input  wire        irq_stat_setnrst,
	input  wire [ 6:0] irq_stat_wr_sel,
	input  wire        irq_stat_wr_stb,
	//
	output reg  [ 6:0] irq_ena,
	//
	input  wire        irq_ena_setnrst,
	input  wire [ 6:0] irq_ena_wr_sel,
	input  wire        irq_ena_wr_stb
);

	wire intend;

	reg [9:0] intctr;

	reg [1:0] wr;


	wire [6:0] irq_act;
	wire [6:0] irq_autoclr;




`ifdef SIMULATE
	initial
	begin
		intctr = 10'b1100000000;
	end
`endif

	always @(posedge fclk)
		wr[1:0] <= { wr[0], wait_n };

	always @(posedge fclk)
	begin
		if( vbl_start )
			intctr <= 10'd0;
		else if( !intctr[9:8] && wr[1] )
			intctr <= intctr + 10'd1;
	end


	assign intend = intctr[9:8] || ( (!iorq_n) && (!m1_n) && zneg );


	always @(posedge fclk)
	begin
		if( vbl_start )
			int_n <= 1'b0;
		else if( intend )
			int_n <= 1'bZ;
	end





	// enhanced IRQs control

	// enable
	always @(posedge fclk, negedge rst_n)
	if( !rst_n )
		irq_ena <= 7'd0;
	else if( !irq_enh )
		irq_ena <= 7'd0;
	else if( irq_ena_wr_stb )
		irq_ena <= (irq_ena & (~irq_ena_wr_sel)) | ({7{irq_ena_setnrst}} & irq_ena_wr_sel);

	// status
	assign irq_autoclr = 7'd0; // TODO: implement autoclr feature
	assign irq_act = {5'd0, tmr_start, vbl_start};

	always @(posedge fclk, negedge rst_n)
	if( !rst_n )
		irq_stat <= 7'd0;
	else if( !irq_enh )
		irq_stat <= 7'd0;
	else 
	begin : status_bits
		integer i;
		for(i=0;i<7;i=i+1) begin
			if( irq_act[i] || (irq_ena_wr_stb && irq_ena_setnrst && irq_ena_wr_sel[i]) )
				irq_stat[i] <= 1'b1;
			else if( irq_autoclr[i] || (irq_ena_wr_stb && !irq_ena_setnrst && irq_ena_wr_sel[i]) )
				irq_stat[i] <= 1'b0;
		end
	end



endmodule

