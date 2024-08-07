`include "../include/tune.v"

module top(

	// clocks
	input fclk,
	output clkz_out,
	input clkz_in,

	// z80
	input iorq_n,
	input mreq_n,
	input rd_n,
	input wr_n,
	input m1_n,
	input rfsh_n,
	output int_n,
	output nmi_n,
	output wait_n,
	output res,

	inout [7:0] d,
	input [15:0] a,

	// zxbus and related
	output csrom,
	output romoe_n,
	output romwe_n,

	output rompg0_n,
	output dos_n, // aka rompg1
	output rompg2,
	output rompg3,
	output rompg4,

	input iorqge1,
	input iorqge2,
	output iorq1_n,
	output iorq2_n,

	// DRAM
	inout [15:0] rd,
	output [9:0] ra,
	output rwe_n,
	output rucas_n,
	output rlcas_n,
	output rras0_n,
	output rras1_n,

	// video
	output [1:0] vred,
	output [1:0] vgrn,
	output [1:0] vblu,

	output vhsync,
	output vvsync,
	output vcsync,

	// AY control and audio/tape
	output ay_clk,
	output ay_bdir,
	output ay_bc1,

	output beep,

	// IDE
	output [2:0] ide_a,
	inout [15:0] ide_d,

	output ide_dir,

	input ide_rdy,

	output ide_cs0_n,
	output ide_cs1_n,
	output ide_rs_n,
	output ide_rd_n,
	output ide_wr_n,

	// VG93 and diskdrive
	output vg_clk,

	output vg_cs_n,
	output vg_res_n,

	output vg_hrdy,
	output vg_rclk,
	output vg_rawr,
	output [1:0] vg_a, // disk drive selection
	output vg_wrd,
	output vg_side,

	input step,
	input vg_sl,
	input vg_sr,
	input vg_tr43,
	input rdat_b_n,
	input vg_wf_de,
	input vg_drq,
	input vg_irq,
	input vg_wd,

	// serial links (atmega-fpga, sdcard)
	output sdcs_n,
	output sddo,
	output sdclk,
	input sddi,

	input spics_n,
	input spick,
	input spido,
	output spidi,
	output spiint_n
);

	wire dos;


	wire zclk; // z80 clock for short

	wire rst_n; // global reset

	wire rrdy;
	wire cbeg;
	wire [15:0] rddata;

	wire [4:0] rompg;

	wire [7:0] zports_dout;
	wire zports_dataout;
	wire porthit;


	wire [39:0] kbd_data;
	wire [ 7:0] mus_data;
	wire kbd_stb,mus_xstb,mus_ystb,mus_btnstb,kj_stb;

	wire [ 4:0] kbd_port_data;
	wire [ 4:0] kj_port_data;
	wire [ 7:0] mus_port_data;




	wire [7:0] wait_read,wait_write;
	wire wait_rnw;
	wire wait_start_gluclock;
	wire wait_end;
	wire [7:0] gluclock_addr;
	wire [6:0] waits;




	// config signals
	wire [7:0] not_used;
	wire cfg_vga_on;
	wire set_nmi;
	wire turbo_key;
	wire noturbo_key;




	wire tape_in;

	wire [15:0] ideout;
	wire [15:0] idein;
	wire idedataout;


	wire [7:0] zmem_dout;
	wire zmem_dataout;



	reg [3:0] ayclk_gen;


	wire [7:0] received;
	wire [7:0] tobesent;


	wire intrq,drq;
	wire vg_wrFF;

	wire [1:0] rstrom;




	assign zclk = clkz_in;


	// RESETTER
	wire genrst;

	resetter myrst( .clk(fclk),
	                .rst_in_n(~genrst),
	                .rst_out_n(rst_n) );
	defparam myrst.RST_CNT_SIZE = 6;



	assign nmi_n = nmi_out ? 1'b0 : 1'bZ;

	assign res= ~rst_n;





	assign ide_rs_n = turbo;

	assign ide_d = idedataout ? ideout : 16'hZZZZ;
	assign idein = ide_d;

	assign ide_dir = ~idedataout;





	wire [7:0] p1ffd;
	wire [7:0] p7ffd;
	wire [7:0] peff7;

	wire tape_read;

	wire [2:0] comport_addr;
	wire wait_start_comport;			

	wire cpu_req,cpu_rnw,cpu_wrbsel,cpu_strobe;
	wire [20:0] cpu_addr;
	wire [15:0] cpu_rddata;
	wire [7:0] cpu_wrdata;


	wire cend,pre_cend,go;


	wire sd_start;
	wire [7:0] sd_dataout,sd_datain;



//AY control
	always @(posedge fclk)
	begin
		ayclk_gen <= ayclk_gen + 4'd1;
	end

	assign ay_clk = ayclk_gen[3];


	assign rompg0_n = ~rompg[0];
	assign dos_n    = ~rompg[1];
	assign rompg2   =  prof1;
	assign rompg3   =  prof2;
	assign rompg4   =  1'b0;

	zclock z80clk( .fclk(fclk), .rst_n(rst_n), .zclk(zclk), .rfsh_n(rfsh_n), .zclk_out(clkz_out),
	               .turbo( {1'b0,turbo} ), .pre_cend(pre_cend), .cbeg(cbeg) ); 



	wire [7:0] dout_ram;
	wire ena_ram;
	wire [7:0] dout_ports;
	wire ena_ports;
	wire prof1;
	wire prof2;
	wire [7:0] attr;
	wire [7:0] portFF;
	
	assign portFF = (a[7:0]==8'hff)? ((hpix && vpix)? attr : 8'hFF) : 8'hff;	

	wire [2:0] border;

	wire drive_ff;

	// data bus out: either RAM data or internal ports data or 0xFF with unused ports
	assign d = ena_ram ? dout_ram : ( ena_ports ? dout_ports : ( drive_ff ? portFF : 8'bZZZZZZZZ ) );




	zbus zxbus( .iorq_n(iorq_n), .rd_n(rd_n), .wr_n(wr_n), .m1_n(m1_n),
	            .iorq1_n(iorq1_n), .iorq2_n(iorq2_n), .iorqge1(iorqge1), .iorqge2(iorqge2),
	            .porthit(porthit), .drive_ff(drive_ff) );



	zmem z80mem( .fclk(fclk), .rst_n(rst_n), .zpos(1'b0), .zneg(1'b0),
	             .cend(cend), .pre_cend(pre_cend), .za(a), .zd_in(d),
	             .zd_out(dout_ram), .zd_ena(ena_ram), .m1_n(m1_n),
	             .rfsh_n(rfsh_n), .iorq_n(iorq_n), .mreq_n(mreq_n),
	             .rd_n(rd_n), .wr_n(wr_n),

	.win0_romnram(~p1ffd[0]), 
	.win1_romnram(1'b0),
	.win2_romnram(1'b0),
	.win3_romnram(1'b0),

	.win0_page( p1ffd[0] ? 8'd0 : ( p1ffd[1] ? 8'd02 : {6'h0,dos,p7ffd[4]} )), 
	.win1_page(8'd5),
	.win2_page(8'd2),
	.win3_page( {2'b00,p1ffd[7:6],p1ffd[4],p7ffd[2:0]} ),

	.dos(dos),

	.rompg(rompg),
	.romoe_n(romoe_n),
	.romwe_n(romwe_n),
	.csrom(csrom),

	.cpu_req(cpu_req),
	.cpu_rnw(cpu_rnw),
	.cpu_wrbsel(cpu_wrbsel),
	.cpu_strobe(cpu_strobe),
	.cpu_addr(cpu_addr),
	.cpu_wrdata(cpu_wrdata),
	.cpu_rddata(cpu_rddata) );






	wire [20:0] daddr;
	wire dreq;
	wire drnw;
	wire [15:0] drddata;
	wire [15:0] dwrdata;
	wire [1:0] dbsel;




	dram dramko( .clk(fclk),
	             .rst_n(rst_n),

	             .addr(daddr),
	             .req(dreq),
	             .rnw(drnw),
	             .cbeg(cbeg),
	             .rrdy(drrdy),
	             .rddata(drddata),
	             .wrdata(dwrdata),
	             .bsel(dbsel),

	             .ra(ra),
	             .rd(rd),
	             .rwe_n(rwe_n),
	             .rucas_n(rucas_n),
	             .rlcas_n(rlcas_n),
	             .rras0_n(rras0_n),
	             .rras1_n(rras1_n) );


	wire [1:0] bw;

	wire [20:0] video_addr;
	wire [15:0] video_data;
	wire video_strobe;
	wire video_next;

	arbiter dramarb( .clk(fclk),
	                 .rst_n(rst_n),

	                 .dram_addr(daddr),
	                 .dram_req(dreq),
	                 .dram_rnw(drnw),
	                 .dram_cbeg(cbeg),
	                 .dram_rrdy(drrdy),
	                 .dram_bsel(dbsel),
	                 .dram_rddata(drddata),
	                 .dram_wrdata(dwrdata),

	                 .cend(cend),
	                 .pre_cend(pre_cend),

	                 .go(go),
	                 .bw(bw),

	                 .video_addr(video_addr),
	                 .video_data(video_data),
	                 .video_strobe(video_strobe),
	                 .video_next(video_next),

	                 //.cpu_waitcyc(cpu_waitcyc),
	                 //.cpu_stall(cpu_stall),
	                 .cpu_req(cpu_req),
	                 .cpu_rnw(cpu_rnw),
	                 .cpu_addr(cpu_addr),
	                 .cpu_wrbsel(cpu_wrbsel),
	                 .cpu_wrdata(cpu_wrdata),
	                 .cpu_rddata(cpu_rddata),
	                 .cpu_strobe(cpu_strobe) );


	wire vga_hsync,hsync,hblank,hpix,hsync_start,line_start,hint_start,scanin_start,scanout_start;

	synch horiz_sync( .clk(fclk), .init(1'b0), .cend(cend), .pre_cend(pre_cend),
	                  .hsync(hsync), .hblank(hblank), .hpix(hpix), .hsync_start(hsync_start),
	                  .line_start(line_start), .hint_start(hint_start), .scanin_start(scanin_start) );


	wire vblank,vsync,int_start,vpix;

	syncv vert_sync( .clk(fclk), .hsync_start(hsync_start), .line_start(line_start),
	                 .vblank(vblank), .vsync(vsync), .int_start(int_start),
	                 .vpix(vpix), .hint_start(hint_start) );

	vga_synch vga_synch( .clk(fclk), .hsync_start(hsync_start), .vga_hsync(vga_hsync), .scanout_start(scanout_start) );



	wire [5:0] pixel;

	fetch fecher( .clk(fclk), .cend(cend), .line_start(line_start), .vpix(vpix), .int_start(int_start),
	              .vmode( {peff7[0],peff7[5]} ), .screen(p7ffd[3]), .video_addr(video_addr), .video_data(video_data),
	              .video_strobe(video_strobe), .video_next(video_next), .go(go), .bw(bw), .attr(attr), .pixel(pixel) );




	videoout vidia( .clk(fclk), .pixel(pixel), .border({ border[1],1'b0,border[2],1'b0,border[0],1'b0 }),
	                .hblank(hblank), .vblank(vblank), .hpix(hpix), .vpix(vpix), .hsync(hsync), .vsync(vsync),
	                .vred(vred), .vgrn(vgrn), .vga_hsync(vga_hsync), .vblu(vblu),
	                .vhsync(vhsync), .vvsync(vvsync), .vcsync(vcsync), .hsync_start(hsync_start),
	                .scanin_start(scanin_start), .scanout_start(scanout_start), .cfg_vga_on(cfg_vga_on) );







	slavespi slavespi( .fclk(fclk), .rst_n(rst_n),
	                   .spics_n(spics_n), .spidi(spidi),
	                   .spido(spido), .spick(spick),
	                   .status_in({wait_rnw, waits[6:0]}), .genrst(genrst),
	                   .rstrom(rstrom), .kbd_out(kbd_data),
	                   .kbd_stb(kbd_stb), .mus_out(mus_data),
	                   .mus_xstb(mus_xstb), .mus_ystb(mus_ystb),
	                   .mus_btnstb(mus_btnstb), .kj_stb(kj_stb),
	                   .gluclock_addr(gluclock_addr),
					   .comport_addr (comport_addr),
	                   .wait_write(wait_write),
	                   .wait_read(wait_read),
	                   .wait_rnw(wait_rnw),
	                   .wait_end(wait_end),
	                   .config0( { not_used[7:5], noturbo_key, turbo_key, tape_read, set_nmi, cfg_vga_on} )
	                 );

	zkbdmus zkbdmus( .fclk(fclk), .rst_n(rst_n),
	                 .kbd_in(kbd_data), .kbd_stb(kbd_stb),
	                 .mus_in(mus_data), .mus_xstb(mus_xstb),
	                 .mus_ystb(mus_ystb), .mus_btnstb(mus_btnstb),
	                 .kj_stb(kj_stb), .kj_data(kj_port_data),
	                 .zah(a[15:8]), .kbd_data(kbd_port_data),
	                 .mus_data(mus_port_data)
	               );


	zports porty( .clk(zclk), .fclk(fclk), .rst_n(rst_n), .din(d), .dout(dout_ports), .dataout(ena_ports),
	              .a(a), .iorq_n(iorq_n), .rd_n(rd_n), .wr_n(wr_n), .porthit(porthit),
	              .ay_bdir(ay_bdir), .ay_bc1(ay_bc1), .border(border), .beep(beep),
	              .p7ffd(p7ffd), .p1ffd(p1ffd), .peff7(peff7), .mreq_n(mreq_n), .m1_n(m1_n), .dos(dos),
	              .rstrom(rstrom), .vg_intrq(intrq), .vg_drq(drq), .vg_wrFF(vg_wrFF),
	              .vg_cs_n(vg_cs_n), .sd_start(sd_start), .sd_dataout(sd_dataout),
	              .sd_datain(sd_datain), .sdcs_n(sdcs_n),
	              .idein(idein), .ideout(ideout), .idedataout(idedataout),
	              .ide_a(ide_a), .ide_cs0_n(ide_cs0_n), .ide_cs1_n(ide_cs1_n),
	              .ide_wr_n(ide_wr_n), .ide_rd_n(ide_rd_n),

	              .keys_in(kbd_port_data),
	              .mus_in(mus_port_data),
	              .kj_in(kj_port_data),
	              .tape_read(tape_read),
				  .turbo_key(turbo_key),
				  .noturbo_key(noturbo_key),

	              .gluclock_addr(gluclock_addr),
				  .comport_addr (comport_addr),
	              .wait_start_gluclock(wait_start_gluclock),
	              .wait_start_comport (wait_start_comport),	

	              .wait_rnw(wait_rnw),
	              .wait_write(wait_write),
	              .wait_read(wait_read),
				  .nmi_key(set_nmi),
				  .nmi_out(nmi_out),
				  .turbo(turbo)
	            );


	zint preryv( .fclk(fclk), .zclk(zclk), .int_start(int_start), .iorq_n(iorq_n), .m1_n(m1_n),
	             .int_n(int_n) );



	zwait zwait( .wait_start_gluclock(wait_start_gluclock),
	             .wait_end(wait_end),
	             .rst_n(rst_n),
				 .wait_start_comport (wait_start_comport),
	             .wait_n(wait_n),
	             .waits(waits),
	             .spiint_n(spiint_n) );

	assign wait_n = 1'bZ;



	wire [1:0] vg_ddrv;
	assign vg_a[0] = vg_ddrv[0] ? 1'b1 : 1'b0; // possibly open drain?
	assign vg_a[1] = vg_ddrv[1] ? 1'b1 : 1'b0;

	vg93 vgshka( .zclk(zclk), .rst_n(rst_n), .fclk(fclk), .vg_clk(vg_clk),
	             .vg_res_n(vg_res_n), .din(d), .intrq(intrq), .drq(drq), .vg_wrFF(vg_wrFF),
	             .vg_hrdy(vg_hrdy), .vg_rclk(vg_rclk), .vg_rawr(vg_rawr), .vg_a(vg_ddrv),
	             .vg_wrd(vg_wrd), .vg_side(vg_side), .step(step), .vg_sl(vg_sl), .vg_sr(vg_sr),
	             .vg_tr43(vg_tr43), .rdat_n(rdat_b_n), .vg_wf_de(vg_wf_de), .vg_drq(vg_drq),
	             .vg_irq(vg_irq), .vg_wd(vg_wd) );




	spi2 zspi( .clock(fclk), .sck(sdclk), .sdo(sddo), .sdi(sddi), .start(sd_start),
	           .speed(2'b00), .din(sd_datain), .dout(sd_dataout) );
	
    pfpzu profrom(.oe_n(!(csrom) | romoe_n),	
				  .a({rompg[1],rompg[0],a[13:2]}),      
				  .a16(prof1),
				  .a17(prof2),
				  .res_n(rst_n)); 

endmodule

