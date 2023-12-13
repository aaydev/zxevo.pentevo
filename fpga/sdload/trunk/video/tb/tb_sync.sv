module tb;

	wire clk, rst_n;

	ckgen ckgen( .clk(clk), .rst_n(rst_n) );

	video_sync video_sync( .clk(clk), .rst_n(rst_n), .vga_on(1'b0), .hsync_polarity(1'b1), .vsync_polarity(1'b1) );





endmodule

