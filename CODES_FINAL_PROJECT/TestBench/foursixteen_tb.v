`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/02/2024 05:40:12 PM
// Design Name: 
// Module Name: foursixteen_tb
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


module foursixteen_tb(

    );
    reg [3:0]I;
    reg [1:0]mode;
    reg start, idle;
    wire [15:0]LED;
    foursixteendecoder Chip(.I(I),.mode(mode),.start(start),.idle(idle),.LED(LED));
    integer z;
    initial begin
        mode = 2'd1; start = 1; idle = 0;
        for (z = 0; z < 16; z = z + 1)
        begin
        I = z;
        #8;
        end
    end
endmodule

