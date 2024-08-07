// megafunction wizard: %RAM: 2-PORT%
// GENERATION: STANDARD
// VERSION: WM1.0
// MODULE: altdpram

// ============================================================
// File Name: video_fontrom.v
// Megafunction Name(s):
// 			altdpram
//
// Simulation Library Files(s):
// 			altera_mf
// ============================================================
// ************************************************************
// THIS IS A WIZARD-GENERATED FILE. DO NOT EDIT THIS FILE!
//
// 7.2 Build 207 03/18/2008 SP 3 SJ Full Version
// ************************************************************


//Copyright (C) 1991-2007 Altera Corporation
//Your use of Altera Corporation's design tools, logic functions
//and other software and tools, and its AMPP partner logic
//functions, and any output files from any of the foregoing
//(including device programming or simulation files), and any
//associated documentation or information are expressly subject
//to the terms and conditions of the Altera Program License
//Subscription Agreement, Altera MegaCore Function License
//Agreement, or other applicable license agreement, including,
//without limitation, that your use is for the sole purpose of
//programming logic devices manufactured by Altera and sold by
//Altera or its authorized distributors.  Please refer to the
//applicable agreement for further details.


// synopsys translate_off
`timescale 1 ps / 1 ps
// synopsys translate_on
module video_fontrom (
	clock,
	data,
	rdaddress,
	rden,
	wraddress,
	wren,
	q);

	input	  clock;
	input	[7:0]  data;
	input	[10:0]  rdaddress;
	input	  rden;
	input	[10:0]  wraddress;
	input	  wren;
	output	[7:0]  q;
`ifndef MODEL_TECH
	wire [7:0] sub_wire0;
	wire [7:0] q = sub_wire0[7:0];

	altdpram	altdpram_component (
				.wren (wren),
				.inclock (clock),
				.rden (rden),
				.data (data),
				.rdaddress (rdaddress),
				.wraddress (wraddress),
				.q (sub_wire0),
				.aclr (1'b0),
				.byteena (1'b1),
				.inclocken (1'b1),
				.outclock (1'b1),
				.outclocken (1'b1),
				.rdaddressstall (1'b0),
				.wraddressstall (1'b0));
	defparam
		altdpram_component.indata_aclr = "OFF",
		altdpram_component.indata_reg = "INCLOCK",
		altdpram_component.intended_device_family = "ACEX1K",
		altdpram_component.lpm_file = "../video/atm.mif",
		altdpram_component.lpm_type = "altdpram",
		altdpram_component.outdata_aclr = "OFF",
		altdpram_component.outdata_reg = "UNREGISTERED",
		altdpram_component.rdaddress_aclr = "OFF",
		altdpram_component.rdaddress_reg = "INCLOCK",
		altdpram_component.rdcontrol_aclr = "OFF",
		altdpram_component.rdcontrol_reg = "INCLOCK",
		altdpram_component.width = 8,
		altdpram_component.widthad = 11,
		altdpram_component.wraddress_aclr = "OFF",
		altdpram_component.wraddress_reg = "INCLOCK",
		altdpram_component.wrcontrol_aclr = "OFF",
		altdpram_component.wrcontrol_reg = "INCLOCK";
`else
	reg [7:0] q;
	
	integer fd;

	reg [7:0] font [0:2047];

	initial
	begin
		fd = $fopen("../video/atm.fnt","rb");

		if( 2048!=$fread(font,fd) )
		begin
			$display("Couldn't load atm.fnt!\n");
			$stop;
		end

		$fclose(fd);
	end

	always @(posedge clock)
	if( wren )
		font[wraddress] <= data;

	always @(posedge clock)
	if( rden )
		q <= font[rdaddress];

`endif

endmodule

// ============================================================
// CNX file retrieval info
// ============================================================
// Retrieval info: PRIVATE: ADDRESSSTALL_A NUMERIC "0"
// Retrieval info: PRIVATE: ADDRESSSTALL_B NUMERIC "0"
// Retrieval info: PRIVATE: BYTEENA_ACLR_A NUMERIC "0"
// Retrieval info: PRIVATE: BYTEENA_ACLR_B NUMERIC "0"
// Retrieval info: PRIVATE: BYTE_ENABLE_A NUMERIC "0"
// Retrieval info: PRIVATE: BYTE_ENABLE_B NUMERIC "0"
// Retrieval info: PRIVATE: BYTE_SIZE NUMERIC "1"
// Retrieval info: PRIVATE: BlankMemory NUMERIC "0"
// Retrieval info: PRIVATE: CLOCK_ENABLE_INPUT_A NUMERIC "0"
// Retrieval info: PRIVATE: CLOCK_ENABLE_INPUT_B NUMERIC "0"
// Retrieval info: PRIVATE: CLOCK_ENABLE_OUTPUT_A NUMERIC "0"
// Retrieval info: PRIVATE: CLOCK_ENABLE_OUTPUT_B NUMERIC "0"
// Retrieval info: PRIVATE: CLRdata NUMERIC "0"
// Retrieval info: PRIVATE: CLRq NUMERIC "0"
// Retrieval info: PRIVATE: CLRrdaddress NUMERIC "0"
// Retrieval info: PRIVATE: CLRrren NUMERIC "0"
// Retrieval info: PRIVATE: CLRwraddress NUMERIC "0"
// Retrieval info: PRIVATE: CLRwren NUMERIC "0"
// Retrieval info: PRIVATE: Clock NUMERIC "0"
// Retrieval info: PRIVATE: Clock_A NUMERIC "0"
// Retrieval info: PRIVATE: Clock_B NUMERIC "0"
// Retrieval info: PRIVATE: ECC NUMERIC "0"
// Retrieval info: PRIVATE: IMPLEMENT_IN_LES NUMERIC "0"
// Retrieval info: PRIVATE: INDATA_ACLR_B NUMERIC "0"
// Retrieval info: PRIVATE: INDATA_REG_B NUMERIC "1"
// Retrieval info: PRIVATE: INIT_FILE_LAYOUT STRING "PORT_A"
// Retrieval info: PRIVATE: INIT_TO_SIM_X NUMERIC "0"
// Retrieval info: PRIVATE: INTENDED_DEVICE_FAMILY STRING "ACEX1K"
// Retrieval info: PRIVATE: JTAG_ENABLED NUMERIC "0"
// Retrieval info: PRIVATE: JTAG_ID STRING "NONE"
// Retrieval info: PRIVATE: MAXIMUM_DEPTH NUMERIC "0"
// Retrieval info: PRIVATE: MEMSIZE NUMERIC "16384"
// Retrieval info: PRIVATE: MEM_IN_BITS NUMERIC "0"
// Retrieval info: PRIVATE: MIFfilename STRING "../video/atm.mif"
// Retrieval info: PRIVATE: OPERATION_MODE NUMERIC "2"
// Retrieval info: PRIVATE: OUTDATA_ACLR_B NUMERIC "0"
// Retrieval info: PRIVATE: OUTDATA_REG_B NUMERIC "1"
// Retrieval info: PRIVATE: RAM_BLOCK_TYPE NUMERIC "0"
// Retrieval info: PRIVATE: READ_DURING_WRITE_MODE_MIXED_PORTS NUMERIC "0"
// Retrieval info: PRIVATE: READ_DURING_WRITE_MODE_PORT_A NUMERIC "3"
// Retrieval info: PRIVATE: READ_DURING_WRITE_MODE_PORT_B NUMERIC "3"
// Retrieval info: PRIVATE: REGdata NUMERIC "1"
// Retrieval info: PRIVATE: REGq NUMERIC "0"
// Retrieval info: PRIVATE: REGrdaddress NUMERIC "1"
// Retrieval info: PRIVATE: REGrren NUMERIC "1"
// Retrieval info: PRIVATE: REGwraddress NUMERIC "1"
// Retrieval info: PRIVATE: REGwren NUMERIC "1"
// Retrieval info: PRIVATE: SYNTH_WRAPPER_GEN_POSTFIX STRING "0"
// Retrieval info: PRIVATE: USE_DIFF_CLKEN NUMERIC "0"
// Retrieval info: PRIVATE: UseDPRAM NUMERIC "1"
// Retrieval info: PRIVATE: VarWidth NUMERIC "0"
// Retrieval info: PRIVATE: WIDTH_READ_A NUMERIC "8"
// Retrieval info: PRIVATE: WIDTH_READ_B NUMERIC "8"
// Retrieval info: PRIVATE: WIDTH_WRITE_A NUMERIC "8"
// Retrieval info: PRIVATE: WIDTH_WRITE_B NUMERIC "8"
// Retrieval info: PRIVATE: WRADDR_ACLR_B NUMERIC "0"
// Retrieval info: PRIVATE: WRADDR_REG_B NUMERIC "1"
// Retrieval info: PRIVATE: WRCTRL_ACLR_B NUMERIC "0"
// Retrieval info: PRIVATE: enable NUMERIC "0"
// Retrieval info: PRIVATE: rden NUMERIC "1"
// Retrieval info: CONSTANT: INDATA_ACLR STRING "OFF"
// Retrieval info: CONSTANT: INDATA_REG STRING "INCLOCK"
// Retrieval info: CONSTANT: INTENDED_DEVICE_FAMILY STRING "ACEX1K"
// Retrieval info: CONSTANT: LPM_FILE STRING "../video/atm.mif"
// Retrieval info: CONSTANT: LPM_TYPE STRING "altdpram"
// Retrieval info: CONSTANT: OUTDATA_ACLR STRING "OFF"
// Retrieval info: CONSTANT: OUTDATA_REG STRING "UNREGISTERED"
// Retrieval info: CONSTANT: RDADDRESS_ACLR STRING "OFF"
// Retrieval info: CONSTANT: RDADDRESS_REG STRING "INCLOCK"
// Retrieval info: CONSTANT: RDCONTROL_ACLR STRING "OFF"
// Retrieval info: CONSTANT: RDCONTROL_REG STRING "INCLOCK"
// Retrieval info: CONSTANT: WIDTH NUMERIC "8"
// Retrieval info: CONSTANT: WIDTHAD NUMERIC "11"
// Retrieval info: CONSTANT: WRADDRESS_ACLR STRING "OFF"
// Retrieval info: CONSTANT: WRADDRESS_REG STRING "INCLOCK"
// Retrieval info: CONSTANT: WRCONTROL_ACLR STRING "OFF"
// Retrieval info: CONSTANT: WRCONTROL_REG STRING "INCLOCK"
// Retrieval info: USED_PORT: clock 0 0 0 0 INPUT NODEFVAL clock
// Retrieval info: USED_PORT: data 0 0 8 0 INPUT NODEFVAL data[7..0]
// Retrieval info: USED_PORT: q 0 0 8 0 OUTPUT NODEFVAL q[7..0]
// Retrieval info: USED_PORT: rdaddress 0 0 11 0 INPUT NODEFVAL rdaddress[10..0]
// Retrieval info: USED_PORT: rden 0 0 0 0 INPUT VCC rden
// Retrieval info: USED_PORT: wraddress 0 0 11 0 INPUT NODEFVAL wraddress[10..0]
// Retrieval info: USED_PORT: wren 0 0 0 0 INPUT VCC wren
// Retrieval info: CONNECT: @data 0 0 8 0 data 0 0 8 0
// Retrieval info: CONNECT: q 0 0 8 0 @q 0 0 8 0
// Retrieval info: CONNECT: @wraddress 0 0 11 0 wraddress 0 0 11 0
// Retrieval info: CONNECT: @rdaddress 0 0 11 0 rdaddress 0 0 11 0
// Retrieval info: CONNECT: @wren 0 0 0 0 wren 0 0 0 0
// Retrieval info: CONNECT: @rden 0 0 0 0 rden 0 0 0 0
// Retrieval info: CONNECT: @inclock 0 0 0 0 clock 0 0 0 0
// Retrieval info: LIBRARY: altera_mf altera_mf.altera_mf_components.all
// Retrieval info: GEN_FILE: TYPE_NORMAL video_fontrom.v TRUE
// Retrieval info: GEN_FILE: TYPE_NORMAL video_fontrom.inc FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL video_fontrom.cmp FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL video_fontrom.bsf TRUE FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL video_fontrom_inst.v FALSE
// Retrieval info: GEN_FILE: TYPE_NORMAL video_fontrom_bb.v FALSE
// Retrieval info: LIB_FILE: altera_mf
