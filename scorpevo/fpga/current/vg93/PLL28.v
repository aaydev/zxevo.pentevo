module pll28(
    input   wire        clk,   // 28mhz
    input   wire        rddat,
    output  reg         rclk,
    output  reg         rawr
    );

reg [5:0] counter = 0;                     
wire[5:0] delta = 27 - counter; 
wire[5:0] shift = { delta[5], delta[5], delta[4:1] }; // sign div
wire[5:0] inc   = rawr_sr[1:0] == 2'b10 ? shift : 1;

reg [3:0] rawr_sr;

always @ (posedge clk)
begin
    rawr_sr <= { rawr_sr[2:0], rddat };
    rawr <= !(rawr_sr[3] && !rawr_sr[0] ); // rawr 100ns
end

always @ (posedge clk)
begin               
    if (counter < 55)
        counter <= counter + inc;   
    else           
    begin             
        counter <= 0;
        rclk = ~rclk;  
    end
    
end