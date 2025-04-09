`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/02/2024 05:41:04 PM
// Design Name: 
// Module Name: alt_count_tb
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


module alt_count_tb(

    );
    reg Clk,reset,en;
    wire [3:0]count;
    udalt_counter tb(.Clk(Clk),.reset(reset),.en(en),.count(count));
    initial begin
    Clk = 1;
    end
    always #5 Clk = ~Clk;
    initial begin
    reset = 1; en = 1;
    #3 reset = 0;
    #1000 
    $finish;
    end
endmodule

