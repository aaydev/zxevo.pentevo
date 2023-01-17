`timescale 1ns/1ps

`define HALF_CLK (17.857) /* 28.000 MHz */

module ckgen
(
	clk,
	rst_n
);
	output clk;
	output rst_n;

	reg clk = 1'b1;
	reg rst_n = 1'b0;
	
	always
		#`HALF_CLK clk = ~clk;
	
	initial
	begin
		repeat(10) @(posedge clk);
		rst_n <= 1'b1;
	end

endmodule
