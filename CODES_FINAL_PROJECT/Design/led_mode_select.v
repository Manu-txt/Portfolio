`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/02/2024 05:09:44 PM
// Design Name: 
// Module Name: led_mode_select
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


module led_mode_select(
    input [1:0]mode,
    output reg [4:0]compareNUM
    );
    always @(mode) //determines the speed of the LED decoding
    begin
        case(mode)
        2'b01: compareNUM = 5'b10101; //low, 21st bit 10101
        2'b10: compareNUM = 5'b10100; //normal, 20th bit 10100
        2'b11: compareNUM = 5'b10011; //high, 19th bit 10011
        default: compareNUM = 5'dz;
        endcase
    end
endmodule
