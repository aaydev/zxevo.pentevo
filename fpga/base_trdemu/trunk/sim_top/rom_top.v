// full zxevo ROM here. ATM paging: bas48(0)/trdos(1)/bas128(2)/gluk(3)

module rom(
	input [18:0] addr,
	output reg [7:0] data,
	input ce_n
);


	reg [7:0] word;

	integer fd;


	reg [7:0] mem [0:524288];



	initial
	begin
		// init rom
		integer i;
		for(i=0;i<524288;i=i+1)
			mem[i] = 8'hFF;
		
		// load file
		fd = $fopen("/tmp/pentevo_test/ers_test.rom","rb");

		if( 524288!=$fread(mem,fd) )
		begin
			$display("Couldn't load rom!\n");
			$stop;
		end

		$fclose(fd);
	end



	always @*
		word = mem[addr[18:0]];


	always @*
	begin
		if( !ce_n )
			data = word;
		else
			data = 8'bZZZZZZZZ;

	end




endmodule

