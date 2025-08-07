module top
(
	input  wire fclk, // input clock from ICS501 or ext. generator

	output wire fclk_fpga, // buffered/delayed clocks for FPGA and videoDAC
	output wire fclk_vdac, //

	// main inputs
	input  wire [1:0] v_r, // v_r[1] is V_R+, v_r[0] is V_R-
	input  wire [1:0] v_g, // v_g[1] is V_G+, v_g[0] is V_G-
	input  wire [1:0] v_b, // v_b[1] is V_B+, v_b[0] is V_B-

	input  wire       vvsync, // sync signals from FPGA
	input  wire       vhsync,
	input  wire       vcsync,

	// main outputs
	output reg  [3:0] d_r, // D_R[3..0]
	output reg  [3:0] d_g,
	output reg  [3:0] d_b,

	output reg        fl_vs,
	output reg        fl_hs,
	output reg        fl_cs
);
	
	reg [1:0] ddr_r,
	          ddr_g,
	          ddr_b;

	// clock buffering/delay action
	assign fclk_fpga = fclk;
	assign fclk_vdac = fclk;

	always @(posedge fclk) // posedge fclk->(10ns)->posedge fclk_fpgs->(13ns)->change of v_* outputs. So here we latch [1:0] part
	begin
		ddr_r <= v_r;
		ddr_g <= v_g;
		ddr_b <= v_b;
	end

	always @(negedge fclk) // latch [3:2] part, pipeline [1:0] part to outputs, latch sync signals
	begin
		d_r <= {v_r, ddr_r};
		d_g <= {v_g, ddr_g};
		d_b <= {v_b, ddr_b};

		fl_vs <= vvsync;
		fl_hs <= vhsync;
		fl_cs <= vcsync;
	end


endmodule

