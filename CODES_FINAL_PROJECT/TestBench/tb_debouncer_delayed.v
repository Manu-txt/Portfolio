`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/31/2024 06:01:55 PM
// Design Name: 
// Module Name: tb_debouncer_delayed
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module tb_debouncer_delayed(

    );
reg clk;
reg rst;
reg noisy;
wire debounced;

debouncer_delayed GEN(
    .clk(clk),
    .rst(rst),
    .noisy(noisy),
    .debounced(debounced)
    );

initial begin
    clk = 0;
    rst = 1;
    noisy = 0;
end    

always
    #5 clk = ~clk;
    
integer i;        
    
initial begin
    #10
    rst = 0;
    #15
    noisy = 1;
    #400
    noisy = 0;
    #200
for(i = 0; i < 15; i = i+1)
    #5  noisy = ~noisy;
   #100     
   noisy = 1;
   #200
   noisy = 0;
   
end

endmodule
