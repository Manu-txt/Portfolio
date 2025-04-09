`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/29/2024 02:18:49 AM
// Design Name: 
// Module Name: counter_refresh
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


// is a select/refresh counter for the anode controller
module counter_refresh(clk_400Hz, select);
input clk_400Hz;
output reg [2:0] select = 0;
always @(posedge clk_400Hz) // at positive edge of 400 Hz clock
    select <= select + 1; // increment select counter by 1
endmodule