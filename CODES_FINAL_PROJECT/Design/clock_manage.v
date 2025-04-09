`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/02/2024 05:09:58 PM
// Design Name: 
// Module Name: clock_manage
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


module clock_manage(
    input sys_clk,rst,en,
    input [4:0]sw,
    output out_clk
    );
    wire [31:0]inClk;
    sys_counter a(.Clk(sys_clk),.Rst(rst),.En(en),.count(inClk));
    mux_32to1 b(.c(inClk),.sw(sw),.o(out_clk));
endmodule
