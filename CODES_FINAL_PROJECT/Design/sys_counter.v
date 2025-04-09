`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/02/2024 05:12:28 PM
// Design Name: 
// Module Name: sys_counter
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


module sys_counter(
    input Clk,Rst,En,
    output [31:0]count
    );
    reg [31:0]temp = 0;
    always @(posedge Clk)
    begin
    if(Rst)
    temp <= 4'd0;
    else
        if(En)
        temp <= temp + 4'd1;  
    end
    assign count = temp;
endmodule

